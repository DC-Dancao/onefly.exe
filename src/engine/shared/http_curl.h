#ifndef ENGINE_SHARED_HTTP_CURL_H
#define ENGINE_SHARED_HTTP_CURL_H

#include <base/detect.h>
#if !defined(CONF_PLATFORM_EMSCRIPTEN)

#include <engine/http.h>

#include <curl/curl.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <unordered_map>

class CHttpRequestCurl : public IHttpRequest
{
	friend class CHttpCurl;

public:
	CHttpRequestCurl(const char *pUrl);
	~CHttpRequestCurl() override;

	void Header(const char *pNameColonValue) override;

private:
	curl_slist *m_pRequestHeaders = nullptr;

	char m_aErr[CURL_ERROR_SIZE];

	bool m_ResponseHeadersEnded = false;

	bool ConfigureHandle(CURL *pHandle);

	
	
	size_t OnHeader(char *pHeader, size_t HeaderSize);

	
	void OnCompletionInternal(CURL *pHandle, CURLcode Code);

	static int ProgressCallback(void *pUser, double DlTotal, double DlCurr, double UlTotal, double UlCurr);
	static size_t HeaderCallback(char *pData, size_t Size, size_t Number, void *pUser);
	static size_t WriteCallback(char *pData, size_t Size, size_t Number, void *pUser);
};

class CHttpCurl : public IEngineHttp
{
public:
	
	bool Init(std::chrono::milliseconds ShutdownDelay) override;
	void Shutdown() override;
	~CHttpCurl() override;

	
	void Run(std::shared_ptr<IHttpRequest> pRequest) override;
	bool HasIpresolveBug() const override;

private:
	enum EState
	{
		UNINITIALIZED,
		RUNNING,
		ERROR,
	};

	void *m_pThread = nullptr;

	std::mutex m_Lock;
	std::condition_variable m_ConditionVariableInit;
	std::atomic<EState> m_State = UNINITIALIZED;
	std::deque<std::shared_ptr<CHttpRequestCurl>> m_PendingRequests;
	std::unordered_map<CURL *, std::shared_ptr<CHttpRequestCurl>> m_RunningRequests;
	std::chrono::milliseconds m_ShutdownDelay{};
	std::optional<std::chrono::time_point<std::chrono::steady_clock>> m_ShutdownTime;
	std::atomic<bool> m_Shutdown = false;

	
	CURLM *m_pMultiH = nullptr;
	int m_NextTimeout;

	static void ThreadMain(void *pUser);
	void RunLoop();
};

#endif 
#endif 
