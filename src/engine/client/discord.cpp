#include <engine/discord.h>

class CDiscordStub : public IDiscord
{
	void Update(bool Enabled) override {}
	void ClearGameInfo() override {}
	
	void SetGameInfo(const CServerInfo &ServerInfo, const char *pMapName, const char *pPlayerName, const char *pSkinName, bool ShowMap, bool Registered) override {}
	void UpdateServerInfo(const CServerInfo &ServerInfo, const char *pMapName, const char *pPlayerName, const char *pSkinName) override {}
	
	void UpdatePlayerCount(int Count) override {}
};

IDiscord *CreateDiscord()
{
	
	IDiscord *pDiscord = CreateBestClientDiscord();
	
	if(pDiscord)
	{
		return pDiscord;
	}
	return new CDiscordStub();
}
