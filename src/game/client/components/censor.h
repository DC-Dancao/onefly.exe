#ifndef GAME_CLIENT_COMPONENTS_CENSOR_H
#define GAME_CLIENT_COMPONENTS_CENSOR_H
#include <base/lock.h>

#include <engine/console.h>
#include <engine/shared/config.h>
#include <engine/shared/jobs.h>

#include <game/client/component.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

void CensorReplaceWords(char *pBuffer, const std::vector<std::string> &vWords, char Replacement);

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


class CCensor : public CComponent
{
public:
	int Sizeof() const override { return sizeof(*this); }
};

#endif 
