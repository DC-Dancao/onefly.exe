#include "censor.h"

#include <base/log.h>
#include <base/str.h>

#include <engine/engine.h>
#include <engine/external/json-parser/json.h>
#include <engine/http.h>
#include <engine/shared/config.h>

#include <optional>
#include <utility>

void CensorReplaceWords(char *pBuffer, const std::vector<std::string> &vWords, char Replacement)
{
	if(!pBuffer)
		return;

	for(const auto &Word : vWords)
	{
		const char *pEnd = nullptr;
		const char *pStart = pBuffer;

		while(pStart)
		{
			pStart = str_utf8_find_nocase(pStart, Word.c_str(), &pEnd);
			if(!pStart)
				continue;
			if((pStart == pBuffer || str_utf8_isspace(*(pStart - 1))) && (str_utf8_isspace(*(pEnd))))
			{
				while(pStart != pEnd)
				{
					pBuffer[pStart - pBuffer] = Replacement;
					pStart++;
				}
			}
			pStart = pEnd;
		}
	}
}

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

