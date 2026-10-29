 
#include "ai_chat.h"

#include <base/log.h>
#include <base/str.h>
#include <base/time.h>

#include <engine/http.h>
#include <engine/shared/config.h>
#include <engine/shared/json.h>
#include <engine/shared/jsonwriter.h>

#include <game/client/gameclient.h>

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <string>
#include <utility>



static constexpr size_t MAX_CONCURRENT_JOBS = 4;
static constexpr size_t MAX_BACKLOG = 8;

static const char *DEFAULT_AI_PROMPT =
	"You are a player in an online multiplayer game chat. "
	"Reply in the same language as the message you are replying to. "
	"Keep it to one short, casual sentence. "
	"Never use markdown, emojis or line breaks.";








static const char AI_MARKER[] = "\xE2\x80\x8B";


static const char *DEFAULT_AI_TEMPLATE =
	"In-game chat message from \"{sender}\":\n{message}\n\n"
	"Write one short reply addressed to {sender}.";

static void ApplyTemplate(const char *pTemplate, const char *pSender, const char *pMessage, char *pOut, size_t OutSize)
{
	pOut[0] = '\0';
	if(!pTemplate || OutSize == 0)
		return;

	size_t Out = 0;
	for(const char *p = pTemplate; *p != '\0' && Out + 1 < OutSize;)
	{
		const char *pRepl;
		if(str_comp_num(p, "{sender}", 8) == 0)
		{
			pRepl = pSender ? pSender : "";
			p += 8;
		}
		else if(str_comp_num(p, "{message}", 9) == 0)
		{
			pRepl = pMessage ? pMessage : "";
			p += 9;
		}
		else
		{
			pOut[Out++] = *p++;
			continue;
		}
		for(const char *q = pRepl; *q != '\0' && Out + 1 < OutSize; ++q)
			pOut[Out++] = *q;
	}
	pOut[Out] = '\0';
}



static void UnescapeNewlines(const char *pIn, char *pOut, size_t OutSize)
{
	size_t Out = 0;
	for(const char *p = pIn; p && *p != '\0' && Out + 1 < OutSize; ++p)
	{
		if(p[0] == '\\' && p[1] == 'n')
		{
			pOut[Out++] = '\n';
			++p;
		}
		else
		{
			pOut[Out++] = *p;
		}
	}
	pOut[Out] = '\0';
}

static bool StartsWithNoCase(const char *pStr, const char *pPrefix)
{
	for(; *pPrefix != '\0'; ++pStr, ++pPrefix)
	{
		if(*pStr == '\0')
			return false;
		if(std::tolower((unsigned char)*pStr) != std::tolower((unsigned char)*pPrefix))
			return false;
	}
	return true;
}

bool CAiChat::IsMention(const char *pText, const char *pName)
{
	if(!pText || !pName || pName[0] == '\0')
		return false;

	const char *p = str_utf8_skip_whitespaces(pText);
	if(p[0] == '\0')
		return false;

	
	char aAtName[128];
	str_format(aAtName, sizeof(aAtName), "@%s", pName);
	const char *pAt = str_find_nocase(p, aAtName);
	if(pAt != nullptr)
	{
		const char *pAfterAt = str_utf8_skip_whitespaces(pAt + str_length(aAtName));
		if(pAt > p || pAfterAt[0] != '\0')
			return true;
	}

	
	if(!StartsWithNoCase(p, pName))
		return false;

	const char *pAfter = p + str_length(pName);
	if(*pAfter == '\0')
		return false;
	if(strchr(":,;!?. \t)", *pAfter) != nullptr)
		pAfter += 1;
	
	else if(str_comp_num(pAfter, "：", 3) == 0 || str_comp_num(pAfter, "，", 3) == 0 ||
		str_comp_num(pAfter, "。", 3) == 0 || str_comp_num(pAfter, "！", 3) == 0 ||
		str_comp_num(pAfter, "？", 3) == 0 || str_comp_num(pAfter, "、", 3) == 0)
		pAfter += 3;
	else
		return false;

	return str_utf8_skip_whitespaces(pAfter)[0] != '\0';
}

static void StripLeadingMention(char *pText, const char *pName)
{
	if(!pText || !pName || pName[0] == '\0')
		return;

	const char *p = pText;
	while(*p == '@' || *p == ' ')
		++p;

	const int NameLen = str_length(pName);
	if(str_comp_nocase_num(p, pName, NameLen) != 0)
		return;

	const char *pRest = p + NameLen;
	if(str_comp_num(pRest, "：", 3) == 0 || str_comp_num(pRest, "，", 3) == 0)
		pRest += 3; 
	else if(*pRest == ':' || *pRest == ',')
		pRest += 1;
	else
		return; 

	while(*pRest == ' ' || *pRest == '\t')
		++pRest;

	
	
	memmove(pText, pRest, str_length(pRest) + 1);
}

void CAiChat::SanitizeReply(const char *pRaw, const char *pSender, const char *pTrigger, char *pOut, size_t OutSize)
{
	pOut[0] = '\0';
	if(!pRaw || OutSize == 0)
		return;

	
	char aLine[1024] = "";
	int LineLen = 0;
	for(const char *p = pRaw; *p != '\0' && LineLen < (int)sizeof(aLine) - 1; ++p)
	{
		if(*p == '\n' || *p == '\r')
			break;
		aLine[LineLen++] = *p;
	}
	aLine[LineLen] = '\0';

	char aTrimmed[1024];
	str_copy(aTrimmed, str_utf8_skip_whitespaces(aLine), sizeof(aTrimmed));
	int Len = str_length(aTrimmed);
	while(Len > 0 && (aTrimmed[Len - 1] == ' ' || aTrimmed[Len - 1] == '\t'))
		aTrimmed[--Len] = '\0';

	
	if(Len >= 2 && aTrimmed[0] == '"' && aTrimmed[Len - 1] == '"')
	{
		aTrimmed[Len - 1] = '\0';
		memmove(aTrimmed, aTrimmed + 1, Len - 1);
	}
	while(*aTrimmed == '*' || *aTrimmed == '>' || *aTrimmed == ' ')
		memmove(aTrimmed, aTrimmed + 1, str_length(aTrimmed));

	
	
	StripLeadingMention(aTrimmed, pSender);
	if(aTrimmed[0] != '\0')
		StripLeadingMention(aTrimmed, pTrigger);

	if(aTrimmed[0] == '\0' || !str_utf8_check(aTrimmed))
		return;

	
	
	str_copy(pOut, aTrimmed, OutSize);
}




static void SplitReply(const char *pText, int MaxBytes, char *pOut1, size_t Out1Size, char *pOut2, size_t Out2Size)
{
	pOut1[0] = '\0';
	pOut2[0] = '\0';
	if(!pText || pText[0] == '\0')
		return;

	if(str_length(pText) <= MaxBytes)
	{
		str_copy(pOut1, pText, Out1Size);
		return;
	}

	
	
	
	const int BreakBudget = std::max(8, MaxBytes - 24);
	str_copy(pOut1, pText, BreakBudget + 1);
	str_utf8_fix_truncation(pOut1);

	
	
	auto IsBreak = [](int Code) {
		return Code == ' ' || Code == '\t' || Code == '\n' ||
		       Code == ',' || Code == '.' || Code == '!' || Code == '?' || Code == ';' || Code == ':' ||
		       Code == 0x3001 || Code == 0x3002 || 
		       Code == 0xFF0C || Code == 0xFF01 || Code == 0xFF1F || Code == 0xFF1B || Code == 0xFF1A; 
	};
	const int MaxCut = str_length(pOut1);
	int BestCut = MaxCut;
	const char *pScan = pOut1;
	while(*pScan != '\0')
	{
		const int Code = str_utf8_decode(&pScan);
		const int CharEnd = (int)(pScan - pOut1);
		if(CharEnd > MaxCut)
			break;
		if(CharEnd >= MaxCut - 36 && IsBreak(Code))
			BestCut = CharEnd;
	}
	pOut1[BestCut] = '\0';

	const int CutBytes = str_length(pOut1);
	char aRest[2048];
	str_copy(aRest, str_utf8_skip_whitespaces(pText + CutBytes), sizeof(aRest));
	str_copy(pOut2, aRest, MaxBytes + 1);
	str_utf8_fix_truncation(pOut2);
}

void CAiChat::StartJob(const char *pSender, const char *pTrigger, const char *pText)
{
	char aSystem[2048];
	UnescapeNewlines(g_Config.m_BcAiChatPrompt[0] != '\0' ? g_Config.m_BcAiChatPrompt : DEFAULT_AI_PROMPT, aSystem, sizeof(aSystem));

	char aTemplate[1024];
	UnescapeNewlines(g_Config.m_BcAiChatTemplate[0] != '\0' ? g_Config.m_BcAiChatTemplate : DEFAULT_AI_TEMPLATE, aTemplate, sizeof(aTemplate));

	char aUser[2048];
	ApplyTemplate(aTemplate, pSender, pText, aUser, sizeof(aUser));

	CJsonStringWriter Json = CJsonStringWriter();
	Json.BeginObject();
	Json.WriteAttribute("model");
	Json.WriteStrValue(g_Config.m_BcAiChatModel);
	Json.WriteAttribute("messages");
	Json.BeginArray();
	Json.BeginObject();
	Json.WriteAttribute("role");
	Json.WriteStrValue("system");
	Json.WriteAttribute("content");
	Json.WriteStrValue(aSystem);
	Json.EndObject();
	Json.BeginObject();
	Json.WriteAttribute("role");
	Json.WriteStrValue("user");
	Json.WriteAttribute("content");
	Json.WriteStrValue(aUser);
	Json.EndObject();
	Json.EndArray();
	Json.WriteAttribute("max_tokens");
	Json.WriteIntValue(g_Config.m_BcAiChatMaxTokens);
	Json.WriteAttribute("temperature");
	Json.WriteFloatValue(g_Config.m_BcAiChatTemperature / 10.0);
	Json.WriteAttribute("stream");
	Json.WriteBoolValue(false);
	
	
	
	
	
	if(str_find_nocase(g_Config.m_BcAiChatModel, "deepseek") != nullptr)
	{
		Json.WriteAttribute("thinking");
		Json.BeginObject();
		Json.WriteAttribute("type");
		Json.WriteStrValue("disabled");
		Json.EndObject();
	}
	Json.EndObject();

	const std::string JsonStr = Json.GetOutputString();

	char aUrl[256];
	if(g_Config.m_BcAiChatBaseUrl[0] != '\0')
	{
		char aBase[192];
		str_copy(aBase, g_Config.m_BcAiChatBaseUrl, sizeof(aBase));
		int Len = str_length(aBase);
		while(Len > 0 && aBase[Len - 1] == '/')
			aBase[--Len] = '\0';
		if(str_endswith_nocase(aBase, "/chat/completions") != nullptr)
			str_copy(aUrl, aBase, sizeof(aUrl));
		else
			str_format(aUrl, sizeof(aUrl), "%s/chat/completions", aBase);
	}
	else
		str_copy(aUrl, g_Config.m_BcAiChatEndpoint, sizeof(aUrl));

	std::shared_ptr<IHttpRequest> pRequest = HttpPostJson(aUrl, JsonStr.c_str());
	pRequest->FailOnErrorStatus(false);
	pRequest->LogProgress(HTTPLOG::FAILURE);
	pRequest->Timeout(CTimeout{5000, 20000, 500, 10});
	char aAuth[192];
	str_format(aAuth, sizeof(aAuth), "Bearer %s", g_Config.m_BcAiChatApiKey);
	pRequest->HeaderString("Authorization", aAuth);
	pRequest->HeaderString("Content-Type", "application/json");

	CJob Job;
	Job.m_pHttpRequest = pRequest;
	str_copy(Job.m_aSender, pSender, sizeof(Job.m_aSender));
	str_copy(Job.m_aTrigger, pTrigger ? pTrigger : "", sizeof(Job.m_aTrigger));
	m_vJobs.emplace_back(std::move(Job));

	Http()->Run(pRequest);
}

const char *CAiChat::DefaultPrompt()
{
	return DEFAULT_AI_PROMPT;
}

const char *CAiChat::DefaultTemplate()
{
	return DEFAULT_AI_TEMPLATE;
}

void CAiChat::OnIncomingLine(int ClientId, const char *pSenderName, const char *pText)
{
	if(!g_Config.m_BcAiChatEnable)
		return;
	if(g_Config.m_BcAiChatApiKey[0] == '\0')
		return;
	if(ClientId < 0 || !pSenderName || pSenderName[0] == '\0' || !pText)
		return;

	
	if(!g_Config.m_BcAiChatReplyToAi && str_find(pText, AI_MARKER) != nullptr)
		return;
	for(const int Id : GameClient()->m_aLocalIds)
	{
		if(Id >= 0 && Id == ClientId)
			return;
	}

	const char *pName = g_Config.m_BcAiChatTriggerName[0] != '\0' ? g_Config.m_BcAiChatTriggerName : g_Config.m_PlayerName;
	if(pName[0] == '\0')
		return;
	if(!IsMention(pText, pName))
		return;

	
	
	if(m_vJobs.size() >= MAX_CONCURRENT_JOBS)
	{
		if(m_vBacklog.size() < MAX_BACKLOG)
			m_vBacklog.push_back({pSenderName, pName, pText});
		return;
	}
	if(CooldownActive())
	{
		
		if(m_vBacklog.size() < MAX_BACKLOG)
			m_vBacklog.push_back({pSenderName, pName, pText});
		return;
	}

	StartJob(pSenderName, pName, pText);
	m_LastJobStart = time();
}

bool CAiChat::CooldownActive() const
{
	if(g_Config.m_BcAiChatCooldown <= 0)
		return false;
	return m_LastJobStart != 0 && time() - m_LastJobStart < (int64_t)g_Config.m_BcAiChatCooldown * time_freq() / 1000;
}

void CAiChat::PumpJobs()
{
	while(!m_vBacklog.empty() && m_vJobs.size() < MAX_CONCURRENT_JOBS)
	{
		if(CooldownActive())
			break;

		CBacklog Item = std::move(m_vBacklog.front());
		m_vBacklog.erase(m_vBacklog.begin());
		StartJob(Item.m_Sender.c_str(), Item.m_Trigger.c_str(), Item.m_Text.c_str());
		m_LastJobStart = time();
	}
}

void CAiChat::OnRender()
{
	PumpSendQueue();

	for(auto It = m_vJobs.begin(); It != m_vJobs.end();)
	{
		CJob &Job = *It;
		const EHttpState State = Job.m_pHttpRequest->State();
		if(State == EHttpState::QUEUED || State == EHttpState::RUNNING)
		{
			++It;
			continue;
		}

		if(State == EHttpState::DONE && Job.m_pHttpRequest->StatusCode() == 200)
		{
			char aContent[2048] = "";
			json_value *pRoot = Job.m_pHttpRequest->ResultJson();
			if(pRoot != nullptr)
			{
				const json_value &Content = (*pRoot)["choices"][0]["message"]["content"];
				if(Content.type == json_string)
					str_copy(aContent, Content.u.string.ptr, sizeof(aContent));
				json_value_free(pRoot);
			}

			char aReply[1024] = "";
			SanitizeReply(aContent, Job.m_aSender, Job.m_aTrigger, aReply, sizeof(aReply));
			if(aReply[0] != '\0')
			{
				QueueReplyLine(Job.m_aSender, aReply);
			}
			else
			{
				EchoStatus("未取到有效回复内容，已丢弃");
			}
		}
		else if(State == EHttpState::DONE || State == EHttpState::ERROR)
		{
			log_error("ai_chat", "AI reply request failed (http %d)", Job.m_pHttpRequest->StatusCode());
			EchoStatus("请求失败 (HTTP %d)", Job.m_pHttpRequest->StatusCode());
		}

		It = m_vJobs.erase(It);
	}

	
	PumpJobs();
}

void CAiChat::PumpSendQueue()
{
	if(m_vPending.empty())
		return;
	if(m_NextSendTime != 0 && time() < m_NextSendTime)
		return;

	const std::string Line = m_vPending.front();
	m_vPending.erase(m_vPending.begin());

	GameClient()->m_Chat.SendAiChat(0, Line.c_str(), g_Config.m_BcAiChatAddToHistory != 0);

	
	
	
	
	
	const size_t LineChars = str_utf8_offset_bytes_to_chars(Line.c_str(), str_length(Line.c_str()));
	const int ServerWindowMs = (int)((31 + LineChars) / 32) * 1000 + 500;
	const int64_t IntervalMs = std::max<int64_t>(g_Config.m_BcAiChatCooldown, ServerWindowMs);
	m_NextSendTime = time() + IntervalMs * time_freq() / 1000;
}

void CAiChat::QueueReplyLine(const char *pSender, const char *pReply)
{
	
	
	const int MaxContentBytes = std::max(12, MAX_CHAT_LENGTH - 1 - (str_length(pSender) + 2 + 3));

	if(g_Config.m_BcAiChatSplitLong && str_length(pReply) > MaxContentBytes)
	{
		char aPart1[1024];
		char aPart2[1024];
		SplitReply(pReply, MaxContentBytes, aPart1, sizeof(aPart1), aPart2, sizeof(aPart2));

		char aLine[MAX_CHAT_LENGTH];
		if(aPart1[0] != '\0')
		{
			str_format(aLine, sizeof(aLine), "%s: %s%s", pSender, AI_MARKER, aPart1);
			m_vPending.emplace_back(aLine);
		}
		if(aPart2[0] != '\0')
		{
			str_format(aLine, sizeof(aLine), "%s: %s%s", pSender, AI_MARKER, aPart2);
			m_vPending.emplace_back(aLine);
		}
		return;
	}

	char aReply[MAX_CHAT_LENGTH];
	str_copy(aReply, pReply, MaxContentBytes + 1);
	str_utf8_fix_truncation(aReply);

	char aLine[MAX_CHAT_LENGTH];
	str_format(aLine, sizeof(aLine), "%s: %s%s", pSender, AI_MARKER, aReply);
	m_vPending.emplace_back(aLine);
}

void CAiChat::EchoStatus(const char *pFmt, ...)
{
	if(!g_Config.m_BcAiChatDebug)
		return;

	char aText[256];
	va_list Args;
	va_start(Args, pFmt);
	str_format_v(aText, sizeof(aText), pFmt, Args);
	va_end(Args);

	char aBuf[320];
	str_format(aBuf, sizeof(aBuf), "AICHAT: %s", aText);
	GameClient()->Echo(aBuf);
}

