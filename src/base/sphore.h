 
 

#ifndef BASE_SPHORE_H
#define BASE_SPHORE_H

#include "detect.h"

#include <atomic>

 \
\
\
\


#if defined(CONF_FAMILY_WINDOWS)
 \
\

typedef void *SEMAPHORE;
#elif defined(CONF_PLATFORM_MACOS) || defined(CONF_PLATFORM_IOS)
#include <semaphore.h>
 \
\

typedef sem_t *SEMAPHORE;
#elif defined(CONF_FAMILY_UNIX)
#include <semaphore.h>
 \
\

typedef sem_t SEMAPHORE;
#else
#error not implemented on this platform
#endif

 \
\

void sphore_init(SEMAPHORE *sem);

 \
\

void sphore_wait(SEMAPHORE *sem);

 \
\

void sphore_signal(SEMAPHORE *sem);

 \
\

void sphore_destroy(SEMAPHORE *sem);

 \
\

class CSemaphore
{
	SEMAPHORE m_Sem;
	
	
	std::atomic_int m_Count{0};

public:
	CSemaphore() { sphore_init(&m_Sem); }
	~CSemaphore() { sphore_destroy(&m_Sem); }
	CSemaphore(const CSemaphore &) = delete;
	int GetApproximateValue() { return m_Count.load(); }
	void Wait()
	{
		sphore_wait(&m_Sem);
		m_Count.fetch_sub(1);
	}
	void Signal()
	{
		m_Count.fetch_add(1);
		sphore_signal(&m_Sem);
	}
};

#endif
