// ==========================================================
// Project T5
// 
// Component: t5code
// Sub-component: clientdll
// Purpose: ISteamFriends009 implementation
//
// Initial author: NTAuthority
// Started: 2011-03-31
// ==========================================================

#include "..\stdafx.h"
#include "SteamFriends009.h"

const char *CSteamFriends009::GetPersonaName()
{
	return Auth_GetUsername();
}

void CSteamFriends009::SetPersonaName( const char *pchPersonaName )
{
}

EPersonaState CSteamFriends009::GetPersonaState()
{
	return k_EPersonaStateOnline;
}

int CSteamFriends009::GetFriendCount( int iFriendFlags )
{
	return PM_GetTotalFriends();
}

static bool didRichPresence = false;
CSteamID friendsteamID;
CSteamID CSteamFriends009::GetFriendByIndex( int iFriend, int iFriendFlags )
{
	if (!didRichPresence)
	{
		if (Global::Game::Type == GameType::GAME_TYPE_SP)
		{
			PM_SetRichPresence("currentGame", "iw5sp");
			PM_StoreRichPresence();
		}
		else
		{
			PM_SetRichPresence("currentGame", "iw5mp");
			PM_StoreRichPresence();
		}
		didRichPresence = true;
	}

	return CSteamID(PM_GetFriendGUID(iFriend));
}

EFriendRelationship CSteamFriends009::GetFriendRelationship( CSteamID steamIDFriend )
{
	return k_EFriendRelationshipNone;

	if (PM_GetFriendlyName(steamIDFriend.ConvertToUint64()) != NULL)
	{
		return k_EFriendRelationshipFriend;
	}
}

EPersonaState CSteamFriends009::GetFriendPersonaState( CSteamID steamIDFriend )
{
	EPresenceState presence = PM_GetFriendPresence(steamIDFriend.ConvertToUint64());

	switch (presence)
	{
		case PresenceStateOnline:
			return k_EPersonaStateOnline;
		case PresenceStateAway:
			return k_EPersonaStateAway;
		case PresenceStateExtendedAway:
			return k_EPersonaStateSnooze;
	}

	return k_EPersonaStateOffline;
}

const char *CSteamFriends009::GetFriendPersonaName( CSteamID steamIDFriend )
{
	return PM_GetFriendlyName(steamIDFriend.ConvertToUint64());
}

int CSteamFriends009::GetSmallFriendAvatar( CSteamID steamIDFriend )
{
	return 0;
}

int CSteamFriends009::GetMediumFriendAvatar( CSteamID steamIDFriend )
{
	return 0;
}

int CSteamFriends009::GetLargeFriendAvatar( CSteamID steamIDFriend )
{
	return 0;
}

bool CSteamFriends009::GetFriendGamePlayed( CSteamID steamIDFriend, FriendGameInfo_t *pFriendGameInfo )
{
	const char* friendGame = PM_GetFriendRichPresence(steamIDFriend.ConvertToUint64(), "currentGame");

	if (friendGame && !stricmp(friendGame, "iw5mp"))
	{
		pFriendGameInfo->m_gameID = CGameID(42690); //42680
		pFriendGameInfo->m_steamIDLobby = CSteamID( steamIDFriend.GetAccountID(), 0x40000, k_EUniversePublic, k_EAccountTypeChat );
	}

	if (friendGame && !stricmp(friendGame, "iw5sp"))
	{
		pFriendGameInfo->m_gameID = CGameID(42680); //
		pFriendGameInfo->m_steamIDLobby = CSteamID(steamIDFriend.GetAccountID(), 0x40000, k_EUniversePublic, k_EAccountTypeChat);
	}

	return true;
}

const char *CSteamFriends009::GetFriendPersonaNameHistory( CSteamID steamIDFriend, int iPersonaName )
{


	return "";
}

bool CSteamFriends009::HasFriend( CSteamID steamIDFriend, int iFriendFlags )
{


	return true;
}

int CSteamFriends009::GetClanCount()
{


	return 0;
}

CSteamID CSteamFriends009::GetClanByIndex( int iClan )
{


	return CSteamID();
}

const char *CSteamFriends009::GetClanName( CSteamID steamIDClan )
{


	return "PlusMaster";
}

const char *CSteamFriends009::GetClanTag( CSteamID steamIDClan )
{
	return "PM";
}

int CSteamFriends009::GetFriendCountFromSource( CSteamID steamIDSource )
{


	return 0;
}

CSteamID CSteamFriends009::GetFriendFromSourceByIndex( CSteamID steamIDSource, int iFriend )
{


	return CSteamID();
}

bool CSteamFriends009::IsUserInSource( CSteamID steamIDUser, CSteamID steamIDSource )
{


	return false;
}

void CSteamFriends009::SetInGameVoiceSpeaking( CSteamID steamIDUser, bool bSpeaking )
{

}

void CSteamFriends009::ActivateGameOverlay( const char *pchDialog )
{
	LoadLibrary("gameoverlayrenderer.dll");
}

void CSteamFriends009::ActivateGameOverlayToUser( const char *pchDialog, CSteamID steamID )
{

}

void CSteamFriends009::ActivateGameOverlayToWebPage( const char *pchURL )
{

}


void CSteamFriends009::ActivateGameOverlayToStore( AppId_t nAppID )
{
}

void CSteamFriends009::SetPlayedWith( CSteamID steamIDUserPlayedWith )
{

}

void CSteamFriends009::ActivateGameOverlayInviteDialog( CSteamID steamIDLobby ) {}

bool CSteamFriends009::RequestUserInformation( CSteamID steamIDUser, bool bRequireNameOnly )
{
	return false;
}

SteamAPICall_t CSteamFriends009::RequestClanOfficerList( CSteamID steamIDClan )
{
	return 0;
}

CSteamID CSteamFriends009::GetClanOwner(CSteamID steamIDClan)
{
	return CSteamID(0, 1, k_EUniversePublic, k_EAccountTypeIndividual);
}/*CSteamID GetClanOwner( CSteamID steamIDClan );*/

int CSteamFriends009::GetClanOfficerCount( CSteamID steamIDClan )
{
	return 0;
}

CSteamID CSteamFriends009::GetClanOfficerByIndex(CSteamID steamIDClan, int iOfficer)
{
	return CSteamID(0, 1, k_EUniversePublic, k_EAccountTypeIndividual);
}

EUserRestriction CSteamFriends009::GetUserRestrictions()
{
	return k_nUserRestrictionNone;
}

bool CSteamFriends009::SetRichPresence( const char *pchKey, const char *pchValue )
{
	Log::Debug("SteamFriends", "SetRichPresence %s %s", pchKey, pchValue);
	PM_SetRichPresence(pchKey, pchValue);
	PM_StoreRichPresence();
	return true;
}
void CSteamFriends009::ClearRichPresence()
{
	PM_SetRichPresence("connect", NULL);
	PM_SetRichPresenceBody(NULL);
	PM_StoreRichPresence();
}
const char *CSteamFriends009::GetFriendRichPresence( CSteamID steamIDFriend, const char *pchKey )
{
	Log::Debug("SteamFriends", "GetFriendRichPresence %s", pchKey);
	return PM_GetFriendRichPresence(steamIDFriend.ConvertToUint64(), pchKey);
}
int CSteamFriends009::GetFriendRichPresenceKeyCount( CSteamID steamIDFriend )
{
	return 0;
}
const char *CSteamFriends009::GetFriendRichPresenceKeyByIndex( CSteamID steamIDFriend, int iKey )
{
	return "a";
}

bool CSteamFriends009::InviteUserToGame( CSteamID steamIDFriend, const char *pchConnectString )
{
	return false;
}

int CSteamFriends009::GetCoplayFriendCount()
{
	return 0;
}
CSteamID CSteamFriends009::GetCoplayFriend(int iCoplayFriend)
{
	return CSteamID(0, 1, k_EUniversePublic, k_EAccountTypeIndividual);
}
int CSteamFriends009::GetFriendCoplayTime( CSteamID steamIDFriend )
{
	return 0;
}
AppId_t CSteamFriends009::GetFriendCoplayGame( CSteamID steamIDFriend )
{
	return 0;
}