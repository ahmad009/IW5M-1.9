// This is the main DLL file.

#include "stdafx.h"
#include "PlusIW5.h"

void Anticode();
void Authenticate();
void FindAddresses();
void PatchIAT();
void Patch_IW5MP();
void Patch_IW5SP();
void Patch_IW5DS();

GameType Global::Game::Type = GAME_TYPE_ERROR;

static BYTE originalCode[5];
static PBYTE originalEP = 0;


void Main_UnprotectModule(HMODULE hModule)
{
	PIMAGE_DOS_HEADER header = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + header->e_lfanew);

	// unprotect the entire PE image
	SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
	DWORD oldProtect;
	VirtualProtect((LPVOID)hModule, size, PAGE_EXECUTE_READWRITE, &oldProtect);
}

void Main_DoInit()
{
	// unprotect our entire PE image
	HMODULE hModule;
	if (SUCCEEDED(GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)Main_DoInit, &hModule)))
	{
		Main_UnprotectModule(hModule);
	}

	if (*(DWORD*)0x401B40 == 0x4304720C)
		Global::Game::Type = GameType::GAME_TYPE_MP;
	else if (*(DWORD*)0x401B40 == 0x22E8A300)
		Global::Game::Type = GameType::GAME_TYPE_SP;
	else
		Global::Game::Type = GameType::GAME_TYPE_DEDI;

	FindAddresses();
	PatchIAT();
	DumpHandler::Initialize();


	if (Global::Game::Type == GameType::GAME_TYPE_MP)
	{
#ifndef NoAuth
		Authenticate();
#endif // !NoAuth

		Patch_IW5MP();
	}
	else if (Global::Game::Type == GameType::GAME_TYPE_SP)
	{
#ifndef NoAuth
		Authenticate();
#endif // !NoAuth

		Patch_IW5SP();
	}
	else
	{
		Patch_IW5DS();
	}

	// return to the original EP
	memcpy(originalEP, &originalCode, sizeof(originalCode));
	__asm jmp originalEP
}

void Main_SetSafeInit()
{
	// find the entry point for the executable process, set page access, and replace the EP
	HMODULE hModule = GetModuleHandle(NULL); // passing NULL should be safe even with the loader lock being held (according to ReactOS ldr.c)

	if (hModule)
	{
		PIMAGE_DOS_HEADER header = (PIMAGE_DOS_HEADER)hModule;
		PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((DWORD)hModule + header->e_lfanew);

		Main_UnprotectModule(hModule);

		// back up original code
		PBYTE ep = (PBYTE)((DWORD)hModule + ntHeader->OptionalHeader.AddressOfEntryPoint);
		memcpy(originalCode, ep, sizeof(originalCode));

		// patch to call our EP
		int newEP = (int)Main_DoInit - ((int)ep + 5);
		ep[0] = 0xE9; // for some reason this doesn't work properly when run under the debugger
		memcpy(&ep[1], &newEP, 4);

		originalEP = ep;
	}
}

#pragma optimize("", on)

BOOL __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		Log::Init("Plusmaster/Steam_api.log", LogLevel_All);
		Log::Info("SteamApiBase", "Starting up..");

		Main_UnprotectModule(GetModuleHandle(NULL));

		Anticode();
		dw_init();

		Main_SetSafeInit();
	}

	return TRUE;
}