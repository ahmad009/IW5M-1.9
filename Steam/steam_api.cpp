// ==========================================================
// alterOps project
// 
// Component: client
// Sub-component: clientdll
// Purpose: Entry points for SteamAPI
//
// Initial author: NTAuthority
// Started: 2010-11-21
// ==========================================================

#include "..\stdafx.h"
#include <Shlwapi.h>
#include "GSServer.h"
#include "GSClient.h"
//#include "Callbacks.h"

void SteamProxy_Init();
void SteamProxy_RunFrame();
void Patch_IW5Console();

void NPA_StateSet(EExternalAuthState result);

void NPA_PubNews(const char* message)
{
	Log::Debug("PubNews", (char*)message);
	Cbuf_AddText(0, va("%s\n", message));
}

void NPA_KickClient(PMID npID, const char* reason)
{
	// determine the clientnum
	char* clientAddress = (char*)0x4B4D590;

	if (Global::Game::Type == GameType::GAME_TYPE_DEDI)
	{
		clientAddress = (char*)0x49EB690;
	}

	Log::Debug("NPA", "KickClient commanded to kick %llx for %s", npID, reason);

	int i = 0;
	bool found = false;

	int* count = (int*)0x4B4D58C;

	if (Global::Game::Type == GameType::GAME_TYPE_DEDI)
	{
		count = (int*)0x49EB68C;
	}

	Log::Debug("NPA", "%d clients", *count);

	for (i = 0; i < *count; i++)
	{
		if (*clientAddress >= 3)
		{
			int64_t clientID = *(int64_t*)(clientAddress + ((Global::Game::Type == GameType::GAME_TYPE_DEDI) ? 283320 : 283328));

			Log::Debug("NPA", "client %d is %llx", i, clientID);

			if (clientID == npID)
			{
				Log::Debug("NPA", "found him!");

				found = true;
				break;
			}
		}

		clientAddress += (Global::Game::Type != GameType::GAME_TYPE_DEDI) ? 493200 : 493192;
	}

	if (!found)
	{
		Log::Debug("NPA", "found nobody, returning");
		return;
	}

	Log::Debug("NPA", "execing dropClient %d \"%s\"", i, reason);
	Cbuf_AddText(0, va("dropClient %d \"%s\"\n", i, reason));
}

void HandleP2PIM(PMID from, const uint8_t* buf, uint32_t buflen);

void im_received(PMID from, const uint8_t* buf, uint32_t buflen)
{
	uint8_t type = *buf;

	if (type == 4)
	{
		HandleP2PIM(from, buf, buflen);
		return;
	}


	Log::Debug("Messaging", "received IM from %llx", from);
}

// Steam API code
extern "C"
{
	__declspec(dllexport) HSteamPipe __cdecl GetHSteamPipe()
	{
		return NULL;
	}
	__declspec(dllexport) HSteamUser __cdecl GetHSteamUser()
	{
		return NULL;
	}
	__declspec(dllexport) HSteamPipe __cdecl SteamAPI_GetHSteamPipe()
	{
		return NULL;
	}
	__declspec(dllexport) HSteamUser __cdecl SteamAPI_GetHSteamUser()
	{
		return NULL;
	}
	__declspec(dllexport) const char *__cdecl SteamAPI_GetSteamInstallPath()
	{
		return NULL;
	}

	__declspec(dllexport) bool __cdecl SteamAPI_Init()
	{
		PM_Initiate();

		if (!PM_Connect(MASTER_SERVER, 13030))
		{
			Com_Error(1, "Could not connect to NP server at " MASTER_SERVER);
			return false;
		}

		PM_RegisterExternAuthCallback(NPA_StateSet);

		//PM_RegisterMessageCallback(im_received);

		//SteamProxy_Init();
		//SteamProxy_DoThatTwinklyStuff();

		
		//AuthCI_Init();

		// this is to initialize CI
		//static char fakeTicket[16384];
		//GetUserTicket(fakeTicket, sizeof(fakeTicket), 0);

		PMAuthenticateResult* result;

		Sleep(1000);

		PMAsync<PMAuthenticateResult>* async = PM_CreateSessionWithKey(Auth_GetSessionID());
		result = async->Wait();



		if (result->result != AuthenticateResultOK)
		{
			switch (result->result)
			{
			case AuthenticateResultBadDetails:
				Com_Error(1, "Could not authenticate to NP server at " MASTER_SERVER " -- bad details.");
				break;
			case AuthenticateResultAlreadyLoggedIn:
				Com_Error(1, "Could not authenticate to NP server at " MASTER_SERVER " -- already logged in.");
				break;
			case AuthenticateResultBanned:
				Com_Error(1, "Could not authenticate to NP server at " MASTER_SERVER " -- banned.");
				break;
			case AuthenticateResultUnknown:
				Com_Error(1, "Could not authenticate to NP server at " MASTER_SERVER " -- unknown error.");
				break;
			}
		}

		PM_RegisterKickCallback(NPA_KickClient);

		//GameRT
		/*char filename[32];
		filename[0] = '\x47';
		filename[1] = '\x61';
		filename[2] = '\x6D';
		filename[3] = '\x65';
		filename[4] = '\x52';
		filename[5] = '\x54';
		filename[6] = '\x0';*/

		//PlusCI
		/*char filename[32];
		filename[0] = '\x50';
		filename[1] = '\x6C';
		filename[2] = '\x75';
		filename[3] = '\x73';
		filename[4] = '\x43';
		filename[5] = '\x49';
		filename[6] = '\x0';*/

		//LoadGameModule(filename,BUILDNUMBER);


		/*uint8_t Buffer[4*1024*1024];
		PMID myid;
		PM_GetPMID(&myid);
		PMAsync<PMGetUserFileResult>* async1 = PM_GetUserFile("ExtGameRT.dll", myid,Buffer, sizeof(Buffer));
		async1->Wait();

		PMGetUserFileResult* result3 = async1->GetResult();
		if (result3->result == GetFileResultOK)
		{
		/*std::ofstream fp;
		fp.open("ExtGameRT.dll", std::ios::out | std::ios::binary);
		fp.write((char*)fileBuffer, result3->fileSize);
		Sleep(1000);
		}
		else
		{
		Com_Error(1, "there is problem on downloading PlusCI...");
		}*/
		Sleep(1000);
		PM_LoadGameModule("troll", 0);


		char ticket[2048];
		PM_RequestTicket(ticket, sizeof(ticket), 0);

		GSClient_Init();

		//PM_LoadGameModule("troll", 255);

		return true;
	}

	__declspec(dllexport) bool __cdecl SteamAPI_InitSafe()
	{
		return true;
	}

	__declspec(dllexport) char __cdecl SteamAPI_RestartApp()
	{
		return 1;
	}

	__declspec(dllexport) char __cdecl SteamAPI_RestartAppIfNecessary()
	{
		return 0;
	}

	__declspec(dllexport) void __cdecl SteamAPI_RegisterCallResult(CCallbackBase* pResult, SteamAPICall_t APICall)
	{
		Log::Debug("S_API", "RegisterCallResult for call ID %d", APICall);
		//Callbacks::RegisterResult(pResult, APICall);
		CSteamBase::RegisterCallResult(APICall, pResult);
	}

	__declspec(dllexport) void __cdecl SteamAPI_RegisterCallback(CCallbackBase *pCallback, int iCallback)
	{
		CSteamBase::RegisterCallback(pCallback, iCallback);
		//Callbacks::Register(pCallback, iCallback);
	}

	__declspec(dllexport) void __cdecl SteamAPI_RunCallbacks()
	{
		CSteamBase::RunCallbacks();
		PM_RunCallBacks();
		//SteamProxy_RunFrame();
		GSClient_RunFrame();
		//Callbacks::Run();
	}
	__declspec(dllexport) void __cdecl SteamAPI_SetMiniDumpComment(const char *pchMsg)
	{
	}

	__declspec(dllexport) bool __cdecl SteamAPI_SetTryCatchCallbacks(bool bUnknown)
	{
		return bUnknown;
	}
	__declspec(dllexport) void __cdecl SteamAPI_Shutdown()
	{
	}
	__declspec(dllexport) void __cdecl SteamAPI_UnregisterCallResult(CCallbackBase* pResult, SteamAPICall_t APICall)
	{
	}
	__declspec(dllexport) void __cdecl SteamAPI_UnregisterCallback(CCallbackBase *pCallback, int iCallback)
	{
	}

	__declspec(dllexport) void __cdecl SteamAPI_WriteMiniDump(uint32 uStructuredExceptionCode, void* pvExceptionInfo, uint32 uBuildID)
	{
	}

	__declspec(dllexport) ISteamApps* __cdecl SteamApps()
	{
		Log::Debug("S_API", "SteamApps");
		return (ISteamApps*)CSteamBase::GetInterface(INTERFACE_STEAMAPPS004);
		//return (ISteamApps*)g_pSteamAppsEmu;
	}
	__declspec(dllexport) ISteamClient* __cdecl SteamClient()
	{
		Log::Debug("S_API", "SteamClient");
		return NULL;
		//return (ISteamClient*)g_pSteamClientEmu;
	}
	__declspec(dllexport) ISteamContentServer* __cdecl SteamContentServer()
	{
		return NULL;
	}

	__declspec(dllexport) ISteamUtils* __cdecl SteamContentServerUtils()
	{
		return NULL;
	}

	__declspec(dllexport) bool __cdecl SteamContentServer_Init(unsigned int uLocalIP, unsigned short usPort)
	{
		return NULL;
	}

	__declspec(dllexport) void __cdecl SteamContentServer_RunCallbacks()
	{
	}

	__declspec(dllexport) void __cdecl SteamContentServer_Shutdown()
	{
	}

	__declspec(dllexport) ISteamFriends* __cdecl SteamFriends()
	{
		//Log::Debug("S_API", "SteamFriends");
		return (ISteamFriends*)CSteamBase::GetInterface(INTERFACE_STEAMFRIENDS009);
		//return (ISteamFriends*)g_pSteamFriendsEmu;
	}

	__declspec(dllexport) ISteamGameServer* __cdecl SteamGameServer()
	{
		return (ISteamGameServer*)CSteamBase::GetInterface(INTERFACE_STEAMGAMESERVER010);
		//return (ISteamGameServer*)g_pSteamGameServerEmu;
	}

	__declspec(dllexport) ISteamUtils* __cdecl SteamGameServerUtils()
	{
		return (ISteamUtils*)CSteamBase::GetInterface(INTERFACE_STEAMUTILS005);
	}

	__declspec(dllexport) bool __cdecl SteamGameServer_BSecure()
	{
		return true;
	}

	__declspec(dllexport) HSteamPipe __cdecl SteamGameServer_GetHSteamPipe()
	{
		return NULL;
	}

	__declspec(dllexport) HSteamUser __cdecl SteamGameServer_GetHSteamUser()
	{
		return NULL;
	}

	__declspec(dllexport) int32 __cdecl SteamGameServer_GetIPCCallCount()
	{
		return NULL;
	}

	__declspec(dllexport) uint64 __cdecl SteamGameServer_GetSteamID()
	{
		return NULL;
	}

	static bool steamGS_Inited = false;

	__declspec(dllexport) bool __cdecl SteamGameServer_Init(uint32 unIP, uint16 usPort, uint16 usGamePort, uint16 usSpectatorPort, uint16 usQueryPort, EServerMode eServerMode, const char *pchGameDir, const char *pchVersionString)
	{
		if (Global::Game::Type != GameType::GAME_TYPE_DEDI)
		{
			return true;
		}

		if (steamGS_Inited)
		{
			return true;
		}

		Log::Debug("SteamGameServer_Init", "starting...");

		PM_Initiate();

		if (!PM_Connect(MASTER_SERVER, 13030))
		{
			Com_Printf(0, "Could not connect to NP server at " MASTER_SERVER ".");
			return false;
		}

		Log::Debug("SteamGameServer_Init", "authenticating...");
		PMAuthenticateResult* result;
		Sleep(1000);
		PMAsync<PMAuthenticateResult>* async = PM_CreateSessionWithLicense(GetLicenseKey());
		
		Log::Debug("SteamGameServer_Init", "Waiting for result...");

		result = async->Wait();

		Log::Debug("SteamGameServer_Init", "Result Received...");

		if (result->result != AuthenticateResultOK)
		{
			switch (result->result)
			{
			case AuthenticateResultBadDetails:
				Com_Printf(0, "Could not authenticate to NP server at " MASTER_SERVER " -- bad details.");
				return false;
			case AuthenticateResultAlreadyLoggedIn:
				Com_Printf(0, "Could not authenticate to NP server at " MASTER_SERVER " -- already logged in");
				return false;
			case AuthenticateResultBanned:
				Com_Printf(0, "Could not authenticate to NP server at " MASTER_SERVER " -- banned.");
				return false;
			case AuthenticateResultUnknown:
				Com_Printf(0, "Could not authenticate to NP server at " MASTER_SERVER " -- unknown error.");
				return false;
			}
		}

		Log::Debug("SteamGameServer_Init", "authenticated.");

		PM_RegisterPubNewsCallback(NPA_PubNews);

		PM_RegisterKickCallback(NPA_KickClient);

		steamGS_Inited = true;


		PMID npID;
		PM_GetPMID(&npID);

		Log::Debug("SteamGameServer_Init", "GSServer_Init");

		return GSServer_Init(usGamePort, usQueryPort);

		return true;
	}

	__declspec(dllexport) bool __cdecl SteamGameServer_InitSafe(uint32 unIP, uint16 usPort, uint16 usGamePort, EServerMode eServerMode, int nGameAppId, const char *pchGameDir, const char *pchVersionString, unsigned long dongs)
	{
		return true;
	}

	__declspec(dllexport) void __cdecl SteamGameServer_RunCallbacks()
	{
		PM_RunCallBacks();
		GSServer_RunFrame();

		if (Global::Game::Type == GameType::GAME_TYPE_DEDI)
		{
			CSteamBase::RunCallbacks();
		}
	}

	__declspec(dllexport) void __cdecl SteamGameServer_Shutdown()
	{
	}

	__declspec(dllexport) ISteamMasterServerUpdater* __cdecl SteamMasterServerUpdater()
	{
		return (ISteamMasterServerUpdater*)CSteamBase::GetInterface(INTERFACE_STEAMMASTERSERVERUPDATER001);
		//return (ISteamMasterServerUpdater*)g_pSteamMasterServerUpdaterEmu;
	}

	__declspec(dllexport) ISteamMatchmaking* __cdecl SteamMatchmaking()
	{
		//Log::Debug("S_API", "SteamMatchmaking");
		return (ISteamMatchmaking*)CSteamBase::GetInterface(INTERFACE_STEAMMATCHMAKING008);
		//return (ISteamMatchmaking*)g_pSteamMatchMakingEmu;
	}

	__declspec(dllexport) ISteamMatchmakingServers* __cdecl SteamMatchmakingServers()
	{
		return (ISteamMatchmakingServers*)CSteamBase::GetInterface(INTERFACE_STEAMMATCHMAKINGSERVERS002);
	}

	__declspec(dllexport) ISteamNetworking* __cdecl SteamGameServerNetworking()
	{
		//Log::Debug("S_API", "SteamNetworking");
		return (ISteamNetworking*)CSteamBase::GetInterface(INTERFACE_STEAMNETWORKING005);
	}

	__declspec(dllexport) ISteamNetworking* __cdecl SteamNetworking()
	{
		//Log::Debug("S_API", "SteamNetworking");
		return (ISteamNetworking*)CSteamBase::GetInterface(INTERFACE_STEAMNETWORKING005);
	}

	__declspec(dllexport) void* __cdecl SteamRemoteStorage()
	{
		//return g_pSteamRemoteStorageEmu;
		return CSteamBase::GetInterface(INTERFACE_STEAMREMOTESTORAGE002);
	}

	__declspec(dllexport) ISteamUser* __cdecl SteamUser()
	{
		//return (ISteamUser*)g_pSteamUserEmu;
		//Log::Debug("S_API", "SteamUser");
		return (ISteamUser*)CSteamBase::GetInterface(INTERFACE_STEAMUSER016);
	}

	__declspec(dllexport) ISteamUserStats* __cdecl SteamUserStats()
	{
		//return (ISteamUserStats*)g_pSteamUStatsEmu;
		Log::Debug("S_API", "SteamUserStats");
		return (ISteamUserStats*)CSteamBase::GetInterface(INTERFACE_STEAMUSERSTATS010);
	}

	__declspec(dllexport) ISteamUtils* __cdecl SteamUtils()
	{
		//return (ISteamUtils*)g_pSteamUtilsEmu;
		Log::Debug("S_API", "SteamUtils");
		return (ISteamUtils*)CSteamBase::GetInterface(INTERFACE_STEAMUTILS005);
	}

	__declspec(dllexport) HSteamUser __cdecl Steam_GetHSteamUserCurrent()
	{
		return NULL;
	}

	__declspec(dllexport) void __cdecl Steam_RegisterInterfaceFuncs(void *hModule)
	{

	}

	__declspec(dllexport) void __cdecl Steam_RunCallbacks(HSteamPipe hSteamPipe, bool bGameServerCallbacks)
	{

	}

	__declspec(dllexport) void *g_pSteamClientGameServer = NULL;
}
