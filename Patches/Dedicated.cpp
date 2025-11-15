#include "..\stdafx.h"
#include "..\Steam\GSServer.h"
#include <ws2tcpip.h>
#include <mmsystem.h>

void Patch_IW5LoadRawFiles();
void PatchIW5DS_Ranked();
void Patch_IW5Branding();

void RCon_PrintOOB(int socket, int a2, int a3, int a4, int a5, int a6, int a7, char* buffer)
{
	sockaddr_in s;
	memset(&s, 0, sizeof(s));
	s.sin_family = AF_INET;
	s.sin_port = a4;
	*(int*)&s.sin_addr = a3;

	Log::Debug("RCon_PrintOOB", va("%s", buffer));

	GSServer_PrintOOBInternal(socket, a2, a3, a4, a5, a6, a7, buffer, &s);
}

void Patch_IW5DS()
{
	Patch_IW5LoadRawFiles();
	PatchIW5DS_Ranked();
	Patch_IW5Branding();

	//DS463();
	cmd_id = (DWORD*)0x1B425F8;//1B425F8
	cmd_argc = (DWORD*)0x1B2BB14;//1B4263C
	cmd_argv = (DWORD**)0x1B2BB34;//1B4265C

	DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0x44E7B0;
	Dvar_FindVar = (Dvar_FindVar_t)0x5314B0;

	// DW ports
	/**(DWORD*)0x585AA2 = 3078;
	*(DWORD*)0x585B65 = 3078;
	*(DWORD*)0x585C4E = 3078;
	*(DWORD*)0x585CD2 = 3078;*/
	//*(DWORD*)0x5A1D2E = 3078;// MUST BE TESTED

	// bandwidth/cpu check
	*(BYTE*)0x4F63FA = 0x90;
	*(BYTE*)0x4F63FB = 0xE9;

	// allow any IWD to be loaded
	memset((void*)0x526991, 0x90, 6);

	// UDP packet checksum oddity stuff
	*(WORD*)0x53A867 = 0x9090;

	// allow hardcore servers (commonOption.hardcoreMode being on) to freely configure settings, without Activision's BUT HARDCORE HAS TO BE HARDCORE, NOT FAKECORE
	*(BYTE*)0x4F3950 = 0xC3;

	// always request SteamGS auth
	//memset((void*)0x599CB0, 0x90, 6);

	// dedi license version

	*(char*)0x7803B2 = 'x';
	*(char*)0x780360 = 'x';

	// some check in IW5 challenge/response query stuff; likely IP-related
	memset((void*)0x53A2E7, 0x90, 6);
	memset((void*)0x53A354, 0x90, 6);

	// EXE_ERR_UNREGISTERED_CONNECTION checks
	memset((void*)0x4F68D7, 0x90, 6);
	memset((void*)0x4F68FB, 0x90, 6);

	// rcon oob prints
	QCALL(0x4FF8D4, RCon_PrintOOB, QPATCH_CALL);//ok
	QCALL(0x4FF942, RCon_PrintOOB, QPATCH_CALL);//ok
}

void DS463()
{
	cmd_id = (DWORD*)0x1B2BAD0;//1B425F8
	cmd_argc = (DWORD*)0x1B2BB14;//1B4263C
	cmd_argv = (DWORD**)0x1B2BB34;//1B4265C

	// DW ports
	/**(DWORD*)0x585AA2 = 3078;
	*(DWORD*)0x585B65 = 3078;
	*(DWORD*)0x585C4E = 3078;
	*(DWORD*)0x585CD2 = 3078;*/
	//*(DWORD*)0x5A1D2E = 3078;// MUST BE TESTED

	// bandwidth/cpu check
	*(BYTE*)0x4F76FA = 0x90;
	*(BYTE*)0x4F76FB = 0xE9;

	// allow any IWD to be loaded
	memset((void*)0x527C81, 0x90, 6);

	// UDP packet checksum oddity stuff
	*(WORD*)0x53BB37 = 0x9090;

	// allow hardcore servers (commonOption.hardcoreMode being on) to freely configure settings, without Activision's BUT HARDCORE HAS TO BE HARDCORE, NOT FAKECORE
	*(BYTE*)0x4F4C50 = 0xC3;

	// revision number
	*(const char**)0x4DBDD7 = "Welcome to PlusMW3 v12 Build 82.\nVisit http://community.plusmaster.pro/ for more information\n";

	// always request SteamGS auth
	memset((void*)0x59B4A0, 0x90, 6);

	// dedi license version

	*(char*)0x7803BA = 'x';
	*(char*)0x780368 = 'x';
}