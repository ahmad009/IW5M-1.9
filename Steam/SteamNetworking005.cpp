// ==========================================================
// alterIWnet project
// 
// Component: aiw_client
// Sub-component: steam_api
// Purpose: ISteamNetworking005 implementation
//
// Initial author: NTAuthority
// Started: 2010-09-19
// ==========================================================

#include "..\stdafx.h"
#include "SteamNetworking005.h"
#include <queue>

struct P2PPacket
{
	PMID from;
	std::string buf;
};

static std::queue<P2PPacket> _p2pQueue;
void HandleP2PIM(PMID from, const uint8_t* buf, uint32_t buflen)
{
	P2PPacket packet;
	packet.from = from;
	packet.buf = std::string((const char*)buf + 1, buflen);

	_p2pQueue.push(packet);
}

bool CSteamNetworking005::SendP2PPacket( CSteamID steamIDRemote, const void *pubData, uint32 cubData, EP2PSend eP2PSendType, int unk1 ) {
	Log::Debug("SteamNetworking", "SendP2PPacket");
	static char tempMsg[2048];
	tempMsg[0] = 4;
	memcpy(&tempMsg[1], pubData, cubData);

	//PM_SendMessage(steamIDRemote.ConvertToUint64(), (uint8_t*)tempMsg, cubData + 1);

	return true;
}

bool CSteamNetworking005::IsP2PPacketAvailable( uint32 *pcubMsgSize, int unk1 ) {
	//g_Logging.AddToLogFileA("ISteamNetworking005.log", "IsP2PPacketAvailable");
	if (!_p2pQueue.empty())
	{
		*pcubMsgSize = _p2pQueue.front().buf.size();
	}

	//return !_p2pQueue.empty();
	return false;
}

bool CSteamNetworking005::ReadP2PPacket( void *pubDest, uint32 cubDest, uint32 *pcubMsgSize, CSteamID *psteamIDRemote, int unk1 ) {
	Log::Debug("SteamNetworking", "ReadP2PPacket");
	if (!_p2pQueue.empty())
	{
		P2PPacket packet = _p2pQueue.front();
		_p2pQueue.pop();

		memcpy(pubDest, packet.buf.c_str(), min(packet.buf.size(), cubDest));

		*pcubMsgSize = packet.buf.size();
		*psteamIDRemote = CSteamID(packet.from);

		return true;
	}
	return false;
}

bool CSteamNetworking005::AcceptP2PSessionWithUser( CSteamID steamIDRemote ) {
	return false;
}

bool CSteamNetworking005::CloseP2PSessionWithUser( CSteamID steamIDRemote ) {
	return false;
}

bool CSteamNetworking005::GetP2PSessionState( CSteamID steamIDRemote, P2PSessionState_t *pConnectionState ) {
	return false;
}

SNetListenSocket_t CSteamNetworking005::CreateListenSocket( int nVirtualP2PPort, uint32 nIP, uint16 nPort, bool bAllowUseOfPacketRelay ) {
	return NULL;
}

SNetSocket_t CSteamNetworking005::CreateP2PConnectionSocket( CSteamID steamIDTarget, int nVirtualPort, int nTimeoutSec, bool bAllowUseOfPacketRelay ) {
	return NULL;
}

SNetSocket_t CSteamNetworking005::CreateConnectionSocket( uint32 nIP, uint16 nPort, int nTimeoutSec ) {
	return NULL;
}

bool CSteamNetworking005::DestroySocket( SNetSocket_t hSocket, bool bNotifyRemoteEnd ) {
	return false;
}

bool CSteamNetworking005::DestroyListenSocket( SNetListenSocket_t hSocket, bool bNotifyRemoteEnd ) {
	return false;
}

bool CSteamNetworking005::SendDataOnSocket( SNetSocket_t hSocket, void *pubData, uint32 cubData, bool bReliable ) {
	return false;
}

bool CSteamNetworking005::IsDataAvailableOnSocket( SNetSocket_t hSocket, uint32 *pcubMsgSize ) {
	return false;
}

bool CSteamNetworking005::RetrieveDataFromSocket( SNetSocket_t hSocket, void *pubDest, uint32 cubDest, uint32 *pcubMsgSize ) {
	return false;
}

bool CSteamNetworking005::IsDataAvailable( SNetListenSocket_t hListenSocket, uint32 *pcubMsgSize, SNetSocket_t *phSocket ) {
	return false;
}

bool CSteamNetworking005::RetrieveData( SNetListenSocket_t hListenSocket, void *pubDest, uint32 cubDest, uint32 *pcubMsgSize, SNetSocket_t *phSocket ) {
	return false;
}

bool CSteamNetworking005::GetSocketInfo( SNetSocket_t hSocket, CSteamID *pSteamIDRemote, int *peSocketStatus, uint32 *punIPRemote, uint16 *punPortRemote ) {
	return false;
}

bool CSteamNetworking005::GetListenSocketInfo( SNetListenSocket_t hListenSocket, uint32 *pnIP, uint16 *pnPort ) {
	return false;
}

ESNetSocketConnectionType CSteamNetworking005::GetSocketConnectionType( SNetSocket_t hSocket ) {
	return k_ESNetSocketConnectionTypeNotConnected;
}

int CSteamNetworking005::GetMaxPacketSize( SNetSocket_t hSocket ) {
	return 0;
}

bool CSteamNetworking005::CloseP2PChannelWithUser( CSteamID steamIDRemote, int iVirtualPort )
{
	return true;
}

bool CSteamNetworking005::AllowP2PPacketRelay( bool bAllow )
{
	return true;
}