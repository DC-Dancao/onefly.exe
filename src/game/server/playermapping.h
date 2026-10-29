#ifndef GAME_SERVER_PLAYERMAPPING_H
#define GAME_SERVER_PLAYERMAPPING_H

#include <engine/shared/protocol.h>

#include <generated/protocol7.h>

#include <game/teamscore.h>

class CGameContext;
class CPlayer;
















class CPlayerMapping
{
public:
	class CSixupCfg
	{
	public:
		CSixupCfg() :
			m_SkipTimeoutedId(false),
			m_ClearSlots(false)
		{
		}
		bool m_SkipTimeoutedId;
		bool m_ClearSlots;
	};

private:
	class CGameContext *m_pGameServer;
	class CConfig *m_pConfig;
	class IServer *m_pServer;

	
	
	static constexpr int ms_MaxNumSeeOthers = 34;
	
	static constexpr int ms_MaxNumSeeOthersVanilla = 13;
	
	static constexpr int ms_MaxTeamSizePlayerMap = 10;
	
	static constexpr int ms_MaxTotalTeamSizePlayerMap = 30;

	int m_aTeamSizes[NUM_DDRACE_TEAMS];
	bool m_ReserveAnyTeamSlots;
	char m_aSeeOthersName[MAX_NAME_LENGTH];

	class CPlayerMap
	{
	public:
		void Init(int ClientId, CPlayerMapping *pPlayerMapping);
		void InitPlayer(CSixupCfg SixupCfg);
		CPlayerMapping *m_pPlayerMapping;
		CPlayer *Player() const;
		int m_ClientId;
		int m_NumReserved;
		bool m_UpdateTeamsState;
		bool m_aReserved[MAX_CLIENTS];
		bool m_ResortReserved;
		int *m_pMap;
		int *m_pReverseMap;
		void Update();
		void Add(int MapId, int ClientId);
		int Remove(int MapId);
		void InsertNextEmptyOrReplace(int ClientId);
		int MapSize() const;
		
		int m_SeeOthersPage;
		int m_TotalOverhang;
		int m_NumPages;
		int m_NumSeeOthers;
		bool m_aWasSeeOthers[MAX_CLIENTS];
		int m_LastSeeOthersVoteTick;
		bool m_DoSeeOthersByVote;
		void DoSeeOthers();
		void CycleSeeOthers();
		void UpdateSeeOthers() const;
		void ResetSeeOthers();
		int MaxNumSeeOthers();
	} m_aMap[MAX_CLIENTS];
	void UpdatePlayerMap(int ClientId);

public:
	class CGameContext *GameServer() { return m_pGameServer; }
	class CConfig *Config() { return m_pConfig; }
	class IServer *Server() { return m_pServer; }

	void Init(CGameContext *pGameServer);
	void Tick();

	void InitPlayerMap(int ClientId, CSixupCfg SixupCfg = CSixupCfg()) { m_aMap[ClientId].InitPlayer(SixupCfg); }
	void UpdateTeamsState(int ClientId) { m_aMap[ClientId].m_UpdateTeamsState = true; }
	void ForceInsertPlayer(int Insert, int ClientId) { m_aMap[ClientId].InsertNextEmptyOrReplace(Insert); }

	enum class ESeeOthersInd
	{
		NONE = -1,
		PLAYER = 0,
		BUTTON = 1,
	};
	int SeeOthersId(int ClientId) const;
	bool DoSeeOthers(int ClientId, int SelectedId, bool DoByVote = false);
	void ResetSeeOthers(int ClientId);
	int TotalOverhang(int ClientId) const;
	ESeeOthersInd SeeOthersInd(int ClientId, int MapId) const;
	const char *SeeOthersName(int ClientId);
	bool ReserveTeamSlots(int DDTeam, int ClientId) const;
};

#endif
