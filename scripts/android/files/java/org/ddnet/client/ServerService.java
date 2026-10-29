package org.ddnet.client;

import java.io.File;

import androidx.core.app.NotificationCompat;
import androidx.core.app.RemoteInput;
import androidx.core.app.ServiceCompat;

import android.app.*;
import android.content.*;
import android.content.pm.ServiceInfo;
import android.os.*;
import android.util.*;
import android.widget.Toast;

public class ServerService extends Service {

	private static final String NOTIFICATION_CHANNEL_ID = "LOCAL_SERVER_CHANNEL_ID";
	private static final int NOTIFICATION_ID = 1;

	private static final int MESSAGE_CODE_EXECUTE_COMMAND = 1;
	private static final String MESSAGE_EXTRA_COMMAND = "command";

	private static final String INTENT_ACTION_START = "start";
	private static final String INTENT_ACTION_EXECUTE = "execute";
	private static final String INTENT_EXTRA_COMMANDS = "commands";
	private static final String KEY_EXECUTE_TEXT_REPLY = "execute-command-reply";

	static {
		System.loadLibrary("DDNet-Server");
	}

	private class IncomingHandler extends Handler {

		IncomingHandler(Context context) {
			super(context.getMainLooper());
		}

		@Override
		public void handleMessage(Message message) {
			switch(message.what) {
			case MESSAGE_CODE_EXECUTE_COMMAND:
				String command = message.getData().getString(MESSAGE_EXTRA_COMMAND);
				if(command != null) {
					executeCommand(command);
				}
				break;
			default:
				super.handleMessage(message);
				break;
			}
		}
	}

	private Messenger messenger;
	private NotificationManager notificationManager;
	private NativeServerThread thread;
	private boolean stopping = false;

	@Override
	public void onCreate() {
		super.onCreate();

		notificationManager = getSystemService(NotificationManager.class);

		createNotificationChannel();

		ServiceCompat.startForeground(
			this,
			NOTIFICATION_ID,
			createRunningNotification(),
			ServiceInfo.FOREGROUND_SERVICE_TYPE_MANIFEST
		);
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		if(intent != null) {
			String action = intent.getAction();
			if(INTENT_ACTION_START.equals(action)) {
				String[] commands = intent.getStringArrayExtra(INTENT_EXTRA_COMMANDS);
				if(commands != null) {
					thread = new NativeServerThread(this, commands);
					thread.start();
				}
			} else if(INTENT_ACTION_EXECUTE.equals(action)) {
				Bundle remoteInput = RemoteInput.getResultsFromIntent(intent);
				if(remoteInput != null) {
					CharSequence remoteCommand = remoteInput.getCharSequence(KEY_EXECUTE_TEXT_REPLY);
					if(remoteCommand != null) {
						executeCommand(remoteCommand.toString());
					}
					if(!stopping) {
						
						
						notificationManager.notify(NOTIFICATION_ID, createRunningNotification());
					}
				} else {
					String[] commands = intent.getStringArrayExtra(INTENT_EXTRA_COMMANDS);
					if(commands != null) {
						for(String command : commands) {
							executeCommand(command);
						}
					}
				}
			}
		}
		if(thread == null) {
			stopSelf();
		}
		return START_NOT_STICKY;
	}

	public void onDestroy() {
		super.onDestroy();
		executeCommand("shutdown");
		stopForeground(0);
		if(thread != null) {
			try {
				thread.join(2500);
				if(thread.isAlive()) {
					
					System.exit(0);
				}
			} catch (InterruptedException e) {
			}
			thread = null;
		}
	}

	@Override
	public IBinder onBind(Intent intent) {
		messenger = new Messenger(new IncomingHandler(this));
		return messenger.getBinder();
	}

	@Override
	public boolean onUnbind(Intent intent) {
		
		
		
		executeCommand("shutdown");
		return false;
	}

	private void createNotificationChannel() {
		if(Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
			return;
		}
		NotificationChannel channel = new NotificationChannel(
			NOTIFICATION_CHANNEL_ID,
			getString(R.string.server_name),
			NotificationManager.IMPORTANCE_DEFAULT
		);
		channel.setDescription(getString(R.string.server_notification_channel_description));
		notificationManager.createNotificationChannel(channel);
	}

	private Notification createRunningNotification() {
		Intent activityIntent = new Intent(this, ClientActivity.class);

		PendingIntent activityActionIntent = PendingIntent.getActivity(
			this,
			0, 
			activityIntent,
			PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE
		);

		Intent stopIntent = new Intent(this, ServerService.class);
		stopIntent.setAction(INTENT_ACTION_EXECUTE);
		stopIntent.putExtra(INTENT_EXTRA_COMMANDS, new String[] {"shutdown"});

		PendingIntent stopActionIntent = PendingIntent.getService(
			this,
			0, 
			stopIntent,
			PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE
		);

		NotificationCompat.Action stopAction =
			new NotificationCompat.Action.Builder(
					android.R.drawable.ic_menu_view,
					getString(R.string.server_notification_action_stop),
					stopActionIntent
				).setAuthenticationRequired(true) 
				.build();

		Intent executeCommandIntent = new Intent(this, ServerService.class);
		executeCommandIntent.setAction(INTENT_ACTION_EXECUTE);

		PendingIntent executeCommandActionIntent = PendingIntent.getService(
			this,
			0, 
			executeCommandIntent,
			PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_MUTABLE
		);

		RemoteInput remoteInput =
			new RemoteInput.Builder(KEY_EXECUTE_TEXT_REPLY)
				.setLabel(getString(R.string.server_notification_action_run_command))
				.build();

		NotificationCompat.Action executeAction =
			new NotificationCompat.Action.Builder(
					android.R.drawable.ic_menu_view,
					getString(R.string.server_notification_action_run_command),
					executeCommandActionIntent
				).setAuthenticationRequired(true) 
				.addRemoteInput(remoteInput)
				.build();

		
		
		return new NotificationCompat.Builder(this, NOTIFICATION_CHANNEL_ID)
			.setOngoing(true)
			.setAutoCancel(false)
			.setContentTitle(getString(R.string.server_notification_description_default))
			.setSmallIcon(R.mipmap.ic_launcher)
			.setPriority(NotificationCompat.PRIORITY_DEFAULT)
			.setAllowSystemGeneratedContextualActions(false)
			.setForegroundServiceBehavior(NotificationCompat.FOREGROUND_SERVICE_IMMEDIATE)
			.setContentIntent(activityActionIntent) 
			.setDeleteIntent(stopActionIntent) 
			.addAction(stopAction)
			.addAction(executeAction)
			.build();
	}

	private Notification createStoppingNotification() {
		return new NotificationCompat.Builder(this, NOTIFICATION_CHANNEL_ID)
			.setOngoing(true)
			.setContentTitle(getString(R.string.server_notification_description_stopping))
			.setSmallIcon(R.mipmap.ic_launcher)
			.setPriority(NotificationCompat.PRIORITY_DEFAULT)
			.setAllowSystemGeneratedContextualActions(false)
			.build();
	}

	private void executeCommand(String command) {
		if(thread == null) {
			return;
		}
		
		if("shutdown".equalsIgnoreCase(command)) {
			if(stopping) {
				return;
			}
			stopping = true;
			notificationManager.notify(NOTIFICATION_ID, createStoppingNotification());
		}
		NativeServer.executeCommand(command);
	}

	public static Intent createStartIntent(Context context, String[] arguments) {
		Intent intent = new Intent(context, ServerService.class);
		intent.setAction(INTENT_ACTION_START);
		intent.putExtra(INTENT_EXTRA_COMMANDS, arguments);
		return intent;
	}

	public static Message createExecuteCommandMessage(String command) {
		Message message = Message.obtain(null, MESSAGE_CODE_EXECUTE_COMMAND, 0, 0);
		message.getData().putString(MESSAGE_EXTRA_COMMAND, command);
		return message;
	}
}

 \
\
\
\

class NativeServerThread extends Thread {

	private final Context applicationContext;
	private final String[] arguments;

	public NativeServerThread(Context context, String[] arguments) {
		this.applicationContext = context.getApplicationContext();
		this.arguments = arguments;
	}

	@Override
	public void run() {
		File workingDirectory = applicationContext.getExternalFilesDir(null);
		if(workingDirectory == null) {
			new Handler(applicationContext.getMainLooper()).post(() -> {
				Toast.makeText(applicationContext, R.string.server_error_external_files_inaccessible, Toast.LENGTH_LONG).show();
				terminateProcess();
			});
			return;
		}

		int Result = NativeServer.runServer(workingDirectory.getAbsolutePath(), arguments);
		new Handler(applicationContext.getMainLooper()).post(() -> {
			if(Result != 0) {
				Toast.makeText(applicationContext, applicationContext.getString(R.string.server_error_exit_code, Result), Toast.LENGTH_LONG).show();
			}
			terminateProcess();
		});
	}

	private static void terminateProcess() {
		
		
		System.exit(0);
	}
}

 \
\

class NativeServer {

	private NativeServer() {
		throw new AssertionError();
	}

	 \
\
\
\
\
\
\

	public static native int runServer(String workingDirectory, String[] arguments);

	 \
\
\
\

	public static native void executeCommand(String command);
}
