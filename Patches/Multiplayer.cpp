#include "..\stdafx.h"

void Patch_IW5ClanTag();
//void Patch_IW5Materials();
void Patch_IW5Menu();
void Patch_IW5Console();
void Patch_IW5Dvars();
void Patch_IW5FPS();
void Patch_IW5StartupNotify();
void Patch_IW5Branding();
void Patch_IW5UnlockStats();
void Patch_IW5ClientDvar();
void PatchIW5MPRanked();

void Patch_IW5MP()
{
	Patch_IW5ClanTag();
	//Patch_IW5Materials();
	Patch_IW5Menu();
	Patch_IW5Dvars();
	Patch_IW5Console(); 
	Patch_IW5FPS();
	Patch_IW5StartupNotify();
	Patch_IW5Branding();
	Patch_IW5UnlockStats();
	Patch_IW5ClientDvar();
	PatchIW5MPRanked();

	

	

	


	exprint = *(ExConsolePrint*)0x5275F0;
	Com_Error = (Com_Error_t)0x4D8E50;
	Dvar_FindVar = (Dvar_FindVar_t)0x56C390;

	// disable 'files different from the server' and 'iwd sum/name mismatch' errors
	*(BYTE*)0x527442 = 0xEB; // files different
	*(BYTE*)0x53F448 = 0xEB; // iwd sum/name

	// 'is IWD referenced by the server' func
	*(BYTE*)0x504680 = 0xB8; // mov eax,
	*(DWORD*)0x504681 = 1;   // 1\n
	*(BYTE*)0x504685 = 0xC3; // retn

	// always request steam auth
	memset((void*)0x5183FC, 0x90, 6);

	// cardicon/cardtitle unlocked check
	*(BYTE*)0x635760 = 0xC3;

	// cg_fov archive flag
	static float fov90 = 90.0f;

	*(BYTE*)0x51F055 = 1;//0x41
	*(float**)0x51F048 = &fov90;

	// replace 'xlaunchelitestore' with 'commerceShowStore'
	//*(const char**)0x4B8E89 = "commerceShowStore";
	//call(0x4B8EA8, Open, PATCH_CALL);

	/*CallHook serverTagHook;
	serverTagHook.initialize("", (PBYTE)0x6A36B6);
	serverTagHook.installHook((void(*)())0x41F0D0, false);
	*/
	

	//allow colored name
	//QNOP(0x420813, 2);

	//QNOP(0x502895,2);
	//*(BYTE*)0x1D48E3C = 1;
	//*(BYTE*)0x502849 = 48;
	//*(BYTE*)0x5028A7 = 48;
	//*(BYTE*)0x5028E7 = 48;
	//*(BYTE*)0x5028F0 = 48;


	//QCALL(0x420823, _strncpy_hook, QPATCH_CALL);

	//*(BYTE*)0x4EE1C4 = 0xEB;
	//*(BYTE*)0x4529EE = 3;
	//QCALL(0x694C16, sub_5C2460, QPATCH_CALL);
	//memcpy(color_table, default_color_table, sizeof(default_color_table));
}