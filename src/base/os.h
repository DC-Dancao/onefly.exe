#ifndef BASE_OS_H
#define BASE_OS_H

#include "detect.h"

#include <cstddef>

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
\
\

void cmdline_fix(int *argc, const char ***argv);

 \
\
\
\
\
\
\

void cmdline_free(int argc, const char **argv);

 \
\
\
\
\

class CCmdlineFix
{
	int m_Argc;
	const char **m_ppArgv;

public:
	CCmdlineFix(int *pArgc, const char ***pppArgv)
	{
		cmdline_fix(pArgc, pppArgv);
		m_Argc = *pArgc;
		m_ppArgv = *pppArgv;
	}
	~CCmdlineFix()
	{
		cmdline_free(m_Argc, m_ppArgv);
	}
	CCmdlineFix(const CCmdlineFix &) = delete;
};

#if !defined(CONF_PLATFORM_ANDROID)
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

int os_open_link(const char *link);

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

int os_open_file(const char *path);
#endif 

 \
\
\
\
\
\
\
\
\

bool os_version_str(char *version, size_t length);

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
\
\

void os_locale_str(char *locale, size_t length);

#endif
