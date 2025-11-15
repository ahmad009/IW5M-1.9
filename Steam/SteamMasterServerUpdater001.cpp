// ==========================================================
// alterIWnet project
// 
// Component: aiw_client
// Sub-component: steam_api
// Purpose: ISteamMasterServerUpdater001 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-28
// ==========================================================

#include "..\stdafx.h"
#include "SteamMasterServerUpdater001.h"
#include "GSServer.h"

void CSteamMasterServerUpdater001::SetActive( bool bActive )
{
	Log::Debug("SteamMasterServerUpdater", "SetActive %d", bActive);
	GSServer_SetHeartbeatActive(bActive);
}
void CSteamMasterServerUpdater001::SetHeartbeatInterval( int iHeartbeatInterval )
{
	Log::Debug("SteamMasterServerUpdater", "SetHeartbeatInterval %i", iHeartbeatInterval);
}
bool CSteamMasterServerUpdater001::HandleIncomingPacket( const void *pData, int cbData, uint32 srcIP, uint16 srcPort )
{
	Log::Debug("SteamMasterServerUpdater", "HandleIncomingPacket");
	return true;
}
int CSteamMasterServerUpdater001::GetNextOutgoingPacket( void *pOut, int cbMaxOut, uint32 *pNetAdr, uint16 *pPort )
{
	Log::Debug("SteamMasterServerUpdater", "GetNextOutgoingPacket");
	return 0;
}
void CSteamMasterServerUpdater001::SetBasicServerData(unsigned short nProtocolVersion, bool bDedicatedServer,	const char *pRegionName, const char *pProductName, unsigned short nMaxReportedClients, bool bPasswordProtected,	const char *pGameDescription )
{
	Log::Debug("SteamMasterServerUpdater", "SetBasicServerData %i %i %s %s %i %i %s", nProtocolVersion, bDedicatedServer, pRegionName, pProductName, nMaxReportedClients, bPasswordProtected, pGameDescription);
	GSServer_SetHeartbeatActive(true);
}

void CSteamMasterServerUpdater001::ClearAllKeyValues()
{
	Log::Debug("SteamMasterServerUpdater", "ClearAllKeyValues");
}
void CSteamMasterServerUpdater001::SetKeyValue( const char *pKey, const char *pValue )
{
	Log::Debug("SteamMasterServerUpdater", "SetKeyValue %s %s", pKey, pValue);
}

void CSteamMasterServerUpdater001::NotifyShutdown()
{
	Log::Debug("SteamMasterServerUpdater", "NotifyShutdown");
}
bool CSteamMasterServerUpdater001::WasRestartRequested()
{
	Log::Debug("SteamMasterServerUpdater", "WasRestartRequested");
	return false; 
}
void CSteamMasterServerUpdater001::ForceHeartbeat()
{
	Log::Debug("SteamMasterServerUpdater", "ForceHeartbeat");
}
bool CSteamMasterServerUpdater001::AddMasterServer( const char *pServerAddress )
{
	Log::Debug("SteamMasterServerUpdater", "AddMasterServer %s", pServerAddress);
	return true;
}
bool CSteamMasterServerUpdater001::RemoveMasterServer( const char *pServerAddress )
{
	Log::Debug("SteamMasterServerUpdater", "RemoveMasterServer %s", pServerAddress);
	return true;
}
int CSteamMasterServerUpdater001::GetNumMasterServers() { return 1; }
int CSteamMasterServerUpdater001::GetMasterServerAddress( int iServer, char *pOut, int outBufferSize )
{
	Log::Debug("SteamMasterServerUpdater", "GetMasterServerAddress %d", iServer);
	return 0;
}