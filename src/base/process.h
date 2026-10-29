#ifndef BASE_PROCESS_H
#define BASE_PROCESS_H

#include "detect.h"
#include "types.h"

 \
\
\
\


 \
\
\
\
\
\

int process_id();

#if !defined(CONF_PLATFORM_ANDROID)
 \
\
\
\
\
\
\

enum class EShellExecuteWindowState
{
	 \
\

	FOREGROUND,

	 \
\

	BACKGROUND,
};

 \
\
\
\
\
\
\
\
\
\
\

PROCESS process_execute(const char *file, EShellExecuteWindowState window_state, const char **arguments = nullptr, size_t num_arguments = 0);

 \
\
\
\
\
\
\
\

int process_kill(PROCESS process);

 \
\
\
\
\
\
\
\
\

bool process_is_alive(PROCESS process);
#endif 

#endif
