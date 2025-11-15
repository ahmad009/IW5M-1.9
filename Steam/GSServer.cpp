// ==========================================================
// "alterMW3" project
// 
// Component: code
// Sub-component: clientdll
// Purpose: Master server heartbeat/querying code
//
// Initial author: NTAuthority
// Started: 2012-01-28
// ==========================================================

#include "..\Stdafx.h"
#include "GSServer.h"
#include <WS2tcpip.h>

static struct
{
	// game port
	int gamePort;

	// is initialized?
	bool initialized;
	bool masterActive;

	// next heartbeat at
	DWORD nextHeartbeatTime;

	// socket stuff
	SOCKET serverSocket;
	sockaddr_in from;
} g_svs;

bool GSServer_Init(int gamePort, int queryPort)
{
	if (!queryPort)
	{
		return true;
	}

	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (err)
	{
		return false;
	}

	g_svs.serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (g_svs.serverSocket == INVALID_SOCKET)
	{
		Log::Debug("GSServer", "socket() failed - %d", WSAGetLastError());
		return false;
	}

	int initialQueryPort = queryPort;
	bool bindPassed = false;
	while (queryPort < (initialQueryPort + 10))
	{
		sockaddr_in bindAddr;
		memset(&bindAddr, 0, sizeof(bindAddr));
		bindAddr.sin_family = AF_INET;
		bindAddr.sin_port = htons(queryPort);


		dvar_t** net_ip = (dvar_t**)0x5945AB4;

		bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		
		//bindAddr.sin_addr.s_addr = inet_addr("217.60.151.59");

		if (bind(g_svs.serverSocket, (sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
		{
			Log::Error("GSServer", "bind() failed - %d", WSAGetLastError());

			if (WSAGetLastError() == WSAEADDRINUSE)
			{
				queryPort++;
				continue;
			}

			return false;
		}

		bindPassed = true;
		break;
	}

	if (!bindPassed)
	{
		return false;
	}

	ULONG nonBlocking = 1;
	ioctlsocket(g_svs.serverSocket, FIONBIO, &nonBlocking);

	g_svs.gamePort = gamePort;

	g_svs.initialized = true;
	return true;
}

void __cdecl MSG_Init(msg_t *buf, char *data, int length)
{
	/*if (!msgInit)
		MSG_InitHuffman();
	memset((char *)buf, 0, 0x30u);
	buf->data = data;
	buf->maxsize = length;
	buf->readOnly = 0;
	buf->splitData = 0;
	buf->splitSize = 0;
	*/
	memset((void*)data, 0, 0x2Cu);
	//v7 = *(DWORD*)(buf + 135204);
	//v6 = *(DWORD*)(dword_57877EC + 12) == 0;
	*(DWORD*)(data + 8) = length;
	*(DWORD*)(data + 16) = 0x20000;
	*(DWORD*)(data + 4) = 0;
	*(DWORD*)(data + 12) = 0;
	*(DWORD*)(data + 24) = 0;
}

void GSServer_PrintOOBInternal(int socket, int a2, int a3, int a4, int a5, int a6, int a7, char* buffer, sockaddr_in* addr)
{
	static char tempOOBBuffer[4096];
	tempOOBBuffer[0] = -1;
	tempOOBBuffer[1] = -1;
	tempOOBBuffer[2] = -1;
	tempOOBBuffer[3] = -1;

	memcpy(&tempOOBBuffer[4], buffer, strlen(buffer));

	sendto(g_svs.serverSocket, tempOOBBuffer, strlen(buffer) + 4, 0, (sockaddr*)addr, sizeof(*addr));
}

dvar_t* G_FindDvar(const char* dvarName)
{
	return ((dvar_t*(__cdecl*)(const char*))0x5314B0)(dvarName);
}

const char* Dvar_GetValue(dvar_t** dvar)
{
	const char* result = "";

	switch ((*dvar)->type)
	{
	case DVAR_TYPE_BOOL:
		result = va("%i", (*dvar)->current.boolean);
		break;
	case DVAR_TYPE_ENUM:
		result = va("%i", (*dvar)->current.integer);
		break;
	case DVAR_TYPE_INT:
		result = va("%i", (*dvar)->current.integer);
		break;
	case DVAR_TYPE_STRING:
		result = va("%s", (*dvar)->current.string);
		break;
	case DVAR_TYPE_FLOAT:
		result = va("%g", (*dvar)->current.value);
		break;
	default:
		result = NULL;
		break;
	}

	Log::Debug("Dvar_GetValue", "Dvar type: %i, Dvar Name: %s => '%s'", (*dvar)->type, (*dvar)->name, result);

	return result;
}

void HandleGetInfoOOB(char* buf)
{
	int privateClients;
	int privateClientCount;
	int *clientstate;
	int privcl;
	int clients;
	int *v11;
	int v12;

	char infostring[1024];
	infostring[0] = 0;

	char* challenge = strtok(buf, " ");
	challenge = strtok(NULL, " ");

	dvar_t** onlinegame = (dvar_t**)0xF27A00;
	dvar_t** dedicated = (dvar_t**)0x5868448;

	//(*onlinegame)->current.boolean = true;
	//(*dedicated)->current.integer = 0;

	Dvar_GetValue(onlinegame);
	Dvar_GetValue(dedicated);

	dvar_t** sv_hostname = (dvar_t**)0x5787784;
	dvar_t** sv_mapname = (dvar_t**)0x57877B8;
	dvar_t** sv_gametype = (dvar_t**)0x57877AC;
	dvar_t** sv_maxclients = (dvar_t**)0x5787780;
	dvar_t** sv_privateclients = (dvar_t**)0x1FD52FC;
	*(int*)0x62F1F6C = 0;
	int pure = *(int*)0x62F1F6C;
	dvar_t** game = (dvar_t**)0x58575DC;
	dvar_t** con_disabled = (dvar_t**)0x1FAC460;
	dvar_t** pswrd = (dvar_t**)0x17A1C5C;
	dvar_t** ff = (dvar_t**)0x17D7DEC;
	dvar_t** kc = (dvar_t**)0x17D6D4C;
	//dvar_t** st = (dvar_t**)0xF27A08;
	dvar_t** hc = (dvar_t**)0x17D6D74;
	dvar_t** voice = (dvar_t**)0x57877CC;

	/*
	//dedicated
	(*(dvar_t**)0x5868448)->current.integer = 0;

	//onlinegame
	(*(dvar_t**)0xF27A00)->current.boolean = true;

	Dvar_GetValue((dvar_t**)0x5868448);
	Dvar_GetValue((dvar_t**)0xF27A00);
	*/

	int *clientAddress = (int*)0x4A0FE90;
	int maxclients = *(int*)0x4A0FE8C;

	privateClients = (*sv_privateclients)->current.integer;
	privateClientCount = 0;
	if (privateClients > 0)
	{
		clientstate = clientAddress;
		privcl = privateClients;
		do
		{
			if (*clientstate >= 3)
				++privateClientCount;
			clientstate += 123300;
			--privcl;
		} while (privcl);
	}
	clients = privateClientCount;
	if (privateClients < maxclients)
	{
		v11 = &clientAddress[123300 * privateClients];
		v12 = maxclients - privateClients;
		do
		{
			if (*v11 >= 3)
				++clients;
			v11 += 123300;
			--v12;
		} while (v12);
	}

	int svmaxclients = privateClientCount + (*sv_maxclients)->current.integer/* - (*sv_privateclients)->current.integer*/;
	//[GSServeR] infoResponse\challenge\.IX(0FyzA\protocol\19816\checksum\ - 461731637\hostname\ ^ 1NooB ^ 3Clan ^ Server'\mapname\mp_aground_ss\clients\0\sv_maxclients\18\gametype\tdef\pure\1\kc\1\hw\2\mod\0\voice\0
	Info_SetValueForKey(infostring, "challenge", challenge);
	Info_SetValueForKey(infostring, "protocol", va("%i", 20601));
	Info_SetValueForKey(infostring, "hostname", Dvar_GetValue(sv_hostname));
	Info_SetValueForKey(infostring, "mapname", Dvar_GetValue(sv_mapname));
	Info_SetValueForKey(infostring, "gametype", Dvar_GetValue(sv_gametype));
	Info_SetValueForKey(infostring, "clients", va("%i", clients));
	Info_SetValueForKey(infostring, "sv_maxclients", /*Dvar_GetValue(sv_maxclients)*/va("%i", svmaxclients)/*svmaxclients*/);
	Info_SetValueForKey(infostring, "pure", "1");
	Info_SetValueForKey(infostring, "hw", "1");
	Info_SetValueForKey(infostring, "gamename", "PIW5");
	Info_SetValueForKey(infostring, "fs_game", "modernwarfare3");
	Info_SetValueForKey(infostring, "shortversion", "1.9");
	Info_SetValueForKey(infostring, "ff", Dvar_GetValue(ff));
	Info_SetValueForKey(infostring, "kc", Dvar_GetValue(kc));
	Info_SetValueForKey(infostring, "checksum", "0");

	if(strlen(Dvar_GetValue(pswrd)) > 0)
		Info_SetValueForKey(infostring, "pswrd", "1");

	if (Dvar_GetValue(hc) == "1")
		Info_SetValueForKey(infostring, "hc", "1");

	if (Dvar_GetValue(voice) != "")
		Info_SetValueForKey(infostring, "voice", Dvar_GetValue(voice));


	static char tempOOBBuffer[4096];
	tempOOBBuffer[0] = -1;
	tempOOBBuffer[1] = -1;
	tempOOBBuffer[2] = -1;
	tempOOBBuffer[3] = -1;

	const char* buffer = va("infoResponse\n%s", infostring);
	memcpy(&tempOOBBuffer[4], buffer, strlen(buffer));

	Log::Debug("GSServer_HandleOOB", "%s", buffer);

	sendto(g_svs.serverSocket, tempOOBBuffer, strlen(buffer) + 4, 0, (sockaddr*)&g_svs.from, sizeof(*&g_svs.from));
}

Hook::Stomp netOOBPrintHook;
DWORD netOOBPrintHookLoc = 0x4E1020;

void GSServer_PrintOOB(int socket, char* buffer, int a2, int a3, int a4, int a5, int a6, int a7)
{
	GSServer_PrintOOBInternal(socket, a2, a3, a4, a5, a6, a7, buffer, &g_svs.from);
}

typedef void(__cdecl * SV_ConnectionlessPacket_t)(netadr_t from);
SV_ConnectionlessPacket_t SV_ConnectionlessPacket_ = (SV_ConnectionlessPacket_t)0x4FE120;

void SV_ConnectionlessPacket(msg_t* msg, netadr_t from)
{
	__asm
	{
		push edi
		mov edi, msg
	}

	SV_ConnectionlessPacket_(from);

	__asm
	{
		pop edi
	}
}

typedef DWORD(__cdecl * NET_DeferPacketToClient_t)(netadr_t addr, msg_t msg);
NET_DeferPacketToClient_t NET_DeferPacketToClient = (NET_DeferPacketToClient_t)0x4F6B60;

void GSServer_HandleOOB(char* buffer, int length)
{
	Log::Debug("GSServer_HandleOOB", "%s", buffer);

	/*netadr_t adr;
	*(int*)&adr.ip = *(int*)&g_svs.from.sin_addr;
	adr.port = g_svs.from.sin_port;
	adr.type = NA_IP;

	// create a msg_t
	msg_t msg;
	MSG_Init(&msg, buffer, length);
	msg.cursize = length;*/

	if (strstr(buffer, "getinfo"))
	{
		HandleGetInfoOOB(buffer);
	}
}

void GSServer_PollSocket()
{
	char buf[2048];
	memset(buf, 0, sizeof(buf));

	sockaddr_in from;
	memset(&from, 0, sizeof(from));

	int fromlen = sizeof(from);
	int len = recvfrom(g_svs.serverSocket, buf, 2048, 0, (sockaddr*)&from, &fromlen);

	if (len == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error != WSAEWOULDBLOCK)
		{
			Log::Error("GSServer", "recv() failed - %d", error);
		}

		return;
	}

	g_svs.from = from;

	Log::Debug("GSServer_PollSocket", "%i", from);

	if (*(int*)buf == -1)
	{
		GSServer_HandleOOB(buf, len);
	}

	
}

#define	HEARTBEAT_MSEC	120 * 1000
#define	HEARTBEAT_GAME	"PIW5"

void GSServer_Heartbeat()
{
	// if not time yet, don't send anything
	if (GetTickCount() < g_svs.nextHeartbeatTime)
	{
		return;
	}

	g_svs.nextHeartbeatTime = GetTickCount() + HEARTBEAT_MSEC;

	static bool lookedUp;
	static sockaddr_in masterIP;

	Log::Debug("GSServer_Heartbeat", "nextHeartbeatTime");

	if (!lookedUp)
	{
		hostent* host = gethostbyname(MASTER_SERVER);
		Log::Debug("GSServer_Heartbeat", "host");

		if (!host)
		{
			Log::Error("GSServer", "gethostbyname() failed - %d", WSAGetLastError());
			return;
		}

		masterIP.sin_family = AF_INET;
		masterIP.sin_addr.s_addr = *(ULONG*)host->h_addr_list[0];
		masterIP.sin_port = htons(27951);

		lookedUp = true;
	}

	Log::Debug("GSServer_Heartbeat", "lookedUp");

	char message[128];
	_snprintf(message, sizeof(message), "\xFF\xFF\xFF\xFFheartbeatExt " HEARTBEAT_GAME " %d", g_svs.gamePort);

	Log::Debug("GSServer_Heartbeat", "sendto");
	sendto(g_svs.serverSocket, message, strlen(message), 0, (sockaddr*)&masterIP, sizeof(masterIP));
	Log::Debug("GSServer_Heartbeat", "sent");
}

void GSServer_RunFrame()
{
	if (!g_svs.initialized)
	{
		return;
	}

	GSServer_PollSocket();

	if (g_svs.masterActive)
	{
		GSServer_Heartbeat();
	}
}

void GSServer_SetHeartbeatActive(bool active)
{
	if (/*active || */g_svs.masterActive)
	{
		g_svs.nextHeartbeatTime = 0;
		GSServer_Heartbeat();

		g_svs.nextHeartbeatTime = 0;
		GSServer_Heartbeat();
	}

	//*(BYTE*)0x777F70 = 0;
	//dvar_t** onlinegame = (dvar_t**)0xF27A00;
	//(*onlinegame)->current.boolean = true;

	if (active)
	{
		g_svs.nextHeartbeatTime = 0;
	}

	g_svs.masterActive = active;
}