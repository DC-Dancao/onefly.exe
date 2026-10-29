 
#ifndef GAME_CLIENT_COMPONENTS_BESTCLIENT_AI_CHAT_H
#define GAME_CLIENT_COMPONENTS_BESTCLIENT_AI_CHAT_H

#include <game/client/component.h>

#include <memory>
#include <string>
#include <vector>

class IHttpRequest;





class CAiChat : public CComponent
{
	class CJob
	{
	public:
		std::shared_ptr<IHttpRequest> m_pHttpRequest = nullptr;
		char m_aSender[64] = "";
		char m_aTrigger[64] = "";
	};

	
	
	class CBacklog
	{
	public:
		std::string m_Sender;
		std::string m_Trigger;
		std::string m_Text;
	};

	std::vector<CJob> m_vJobs;
	std::vector<CBacklog> m_vBacklog;
	int64_t m_LastJobStart = 0;

	
	
	std::vector<std::string> m_vPending;
	int64_t m_NextSendTime = 0;

	static bool IsMention(const char *pText, const char *pName);
	static void SanitizeReply(const char *pRaw, const char *pSender, const char *pTrigger, char *pOut, size_t OutSize);
	void StartJob(const char *pSender, const char *pTrigger, const char *pText);
	void PumpJobs();
	void PumpSendQueue();
	bool CooldownActive() const;
	void QueueReplyLine(const char *pSender, const char *pReply);

	
	
	
	[[gnu::format(printf, 2, 3)]] void EchoStatus(const char *pFmt, ...);

public:
	int Sizeof() const override { return sizeof(*this); }

	
	
	
	static const char *DefaultPrompt();
	static const char *DefaultTemplate();

	void OnRender() override;

	
	void OnIncomingLine(int ClientId, const char *pSenderName, const char *pText);

	bool IsBusy() const { return !m_vJobs.empty() || !m_vBacklog.empty(); }
};

#endif 
