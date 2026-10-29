#include "interactions.h"

#include <engine/shared/protocol.h>

#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

const CPlayer *CInteractions::GetPlayer(const CGameContext *pGameServer, int ClientId) const
{
	dbg_assert(ClientId >= 0 && ClientId < MAX_CLIENTS, "invalid client id %d", ClientId);
	return pGameServer->m_apPlayers[ClientId];
}

const CCharacter *CInteractions::Character(const CGameContext *pGameServer, int ClientId) const
{
	return pGameServer->GetPlayerChar(ClientId);
}

bool CInteractions::IsSolo(const CGameContext *pGameServer, int ClientId) const
{
	const CCharacter *pChr = Character(pGameServer, ClientId);
	return pChr && pChr->Core()->m_Solo;
}

int CInteractions::GetDDRaceTeam(const CGameContext *pGameServer, int ClientId) const
{
	return pGameServer->GetDDRaceTeam(ClientId);
}

void CInteractions::Init(int OwnerId, uint32_t UniqueOwnerId)
{
	m_OwnerId = OwnerId;
	m_UniqueOwnerId = UniqueOwnerId;
}

void CInteractions::FillOwnerConnected(
	bool OwnerAlive,
	int DDRaceTeam,
	bool Solo,
	bool NoHitOthers,
	bool NoHitSelf)
{
	m_OwnerAlive = OwnerAlive;
	m_DDRaceTeam = DDRaceTeam;
	m_Solo = Solo;
	m_NoHitOthers = NoHitOthers;
	m_NoHitSelf = NoHitSelf;
}

void CInteractions::FillOwnerDisconnected()
{
	m_OwnerAlive = false;
	m_OwnerId = -1;
	
}

bool CInteractions::CanSee(const CGameContext *pGameServer, int ClientId) const
{
	const CPlayer *pPlayer = GetPlayer(pGameServer, ClientId);
	if(!pPlayer)
		return false; 

	if(!(pPlayer->GetTeam() == TEAM_SPECTATORS || pPlayer->IsPaused()))
	{ 
		if(ClientId != m_OwnerId)
		{ 
			if(pPlayer->m_ShowOthers == SHOW_OTHERS_ONLY_TEAM)
			{
				if(GetDDRaceTeam(pGameServer, ClientId) != m_DDRaceTeam && GetDDRaceTeam(pGameServer, ClientId) != TEAM_SUPER)
					return false; 
			}
			else if(pPlayer->m_ShowOthers == SHOW_OTHERS_OFF)
			{
				if(m_Solo)
					return false; 
				if(IsSolo(pGameServer, ClientId))
					return false; 
				if(GetDDRaceTeam(pGameServer, ClientId) != m_DDRaceTeam && GetDDRaceTeam(pGameServer, ClientId) != TEAM_SUPER)
					return false; 
			}
		} 
	}
	else if(pPlayer->SpectatorId() != SPEC_FREEVIEW)
	{ 
		if(pPlayer->SpectatorId() != m_OwnerId)
		{ 
			if(!Character(pGameServer, pPlayer->SpectatorId()))
				return false; 
			if(pPlayer->m_ShowOthers == SHOW_OTHERS_ONLY_TEAM)
			{
				if(GetDDRaceTeam(pGameServer, pPlayer->SpectatorId()) != m_DDRaceTeam && GetDDRaceTeam(pGameServer, pPlayer->SpectatorId()) != TEAM_SUPER)
					return false; 
			}
			else if(pPlayer->m_ShowOthers == SHOW_OTHERS_OFF)
			{
				if(m_Solo)
					return false; 
				if(IsSolo(pGameServer, pPlayer->SpectatorId()))
					return false; 
				if(GetDDRaceTeam(pGameServer, pPlayer->SpectatorId()) != m_DDRaceTeam && GetDDRaceTeam(pGameServer, pPlayer->SpectatorId()) != TEAM_SUPER)
					return false; 
			}
		} 
	}
	else
	{ 
		if(pPlayer->m_SpecTeam)
		{ 
			if(GetDDRaceTeam(pGameServer, ClientId) != m_DDRaceTeam && GetDDRaceTeam(pGameServer, ClientId) != TEAM_SUPER)
				return false; 
		}
	}

	return true;
}

bool CInteractions::CanHit(const CGameContext *pGameServer, int ClientId) const
{
	const CPlayer *pPlayer = GetPlayer(pGameServer, ClientId);
	if(!pPlayer)
		return false;

	if(m_DDRaceTeam && GetDDRaceTeam(pGameServer, ClientId) != m_DDRaceTeam)
		return false;
	if(m_Solo && m_UniqueOwnerId != pPlayer->GetUniqueCid())
		return false;
	if(m_NoHitOthers && m_UniqueOwnerId != pPlayer->GetUniqueCid())
		return false;
	if(m_NoHitSelf && m_UniqueOwnerId == pPlayer->GetUniqueCid())
		return false;

	return true;
}

CClientMask CInteractions::CanSeeMask(const CGameContext *pGameServer) const
{
	if(m_DDRaceTeam == TEAM_SUPER)
	{
		return CClientMask().set();
	}

	CClientMask Mask;
	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		if(CanSee(pGameServer, i))
		{
			Mask.set(i);
		}
	}
	return Mask;
}

CClientMask CInteractions::CanHitMask(const CGameContext *pGameServer) const
{
	if(m_DDRaceTeam == TEAM_SUPER)
	{
		return CClientMask().set();
	}

	CClientMask Mask;
	for(int i = 0; i < MAX_CLIENTS; ++i)
	{
		if(CanHit(pGameServer, i))
		{
			Mask.set(i);
		}
	}
	return Mask;
}

const CCharacter *CInteractions::OwnerCharacter(const CGameContext *pGameServer) const
{
	const CCharacter *pChr = pGameServer->GetPlayerChar(m_OwnerId);
	if(!pChr)
		return nullptr;
	if(pChr->GetPlayer()->GetUniqueCid() != m_UniqueOwnerId)
		return nullptr;
	return pChr;
}
