#include "..\stdafx.h"

Hook::Call dvarGetIntHook;
DWORD dvarGetIntHookLoc = 0x495DA2;

typedef char* (__cdecl * Dvar_GetDefaultString_t)(const char*, const char*);
Dvar_GetDefaultString_t Dvar_GetDefaultString = (Dvar_GetDefaultString_t)0x531600;

static bool lastWasOnlinegame = false;

char* DvarGetIntHookFunc(const char* dvarName, const char* defaultValue)
{
	Log::Debug("", va("%s", dvarName));

	if (lastWasOnlinegame)
	{
		if (!_stricmp(dvarName, "xblive_privatematch"))
		{
			lastWasOnlinegame = false;
			return "0";
		}
	}

	if (!_stricmp(dvarName, "onlinegame"))
	{
		lastWasOnlinegame = true;
	}
	else
	{
		lastWasOnlinegame = false;
	}

	char checkDvar[1024];
	strcpy(checkDvar, "elacspx_rcs");
	strrev(checkDvar);

	if (!_stricmp(dvarName, checkDvar))
	{
		return "1";
	}

	strcpy(checkDvar, "elbane_pxtser_rcs");
	strrev(checkDvar);

	if (!_stricmp(dvarName, checkDvar))
	{
		return "0";
	}

	return Dvar_GetDefaultString(dvarName, defaultValue);
}

char __cdecl sub_10171800(const char *a1)
{
	int v1; // eax
	int v2; // eax
	int v3; // eax
	char result; // al

	Log::Debug("", "%s", a1);

	v1 = strcmp(a1, "onlinegame");
	if (v1)
		v1 = -(v1 < 0) | 1;
	if (!v1)
		goto LABEL_14;
	v2 = strcmp(a1, "xblive_privatematch");
	if (v2)
		v2 = -(v2 < 0) | 1;
	if (!v2)
		goto LABEL_14;
	v3 = strcmp(a1, "dedicated");
	if (v3)
		v3 = -(v3 < 0) | 1;
	if (v3)
		result = 0;
	else
		LABEL_14:
	result = 1;
	return result;
}

const char *Hookkkk(const char *a1)
{
	char *zero = "0";
	if (sub_10171800(a1) != 0)
		__asm push 531606h
	else
		return zero;
	/*__asm
	{
		pusha
		push    eax
		call    sub_10171800
		add     esp, 4
		test    al, al
		push    531606h
		popa
		mov     eax, zero
		retn
	}*/
}

Hook::Call dvarGetHook;
DWORD dvarGetHookLoc = 0x495CF7;// maybe

char* DvarGetHookFunc(const char* dvarName, const char* defaultValue)
{
	//Log::Debug("getDvar", "%s (%s)", dvarName, defaultValue);

	// fuck you, check for ranked dedis. you aren't allowed to know we're a fucking dedi.
	if (!_stricmp(dvarName, "dedicated"))
	{
		return "0";
	}

	if (strstr(dvarName, "_score_") != NULL)
	{
		return "";
	}

	return Dvar_GetDefaultString(dvarName, defaultValue);
}

Hook::Call getPlayerDataClassHook;
DWORD getPlayerDataClassHookLoc = 0x49310D;//changed  4951F0

void GetPlayerDataClassHook(int stuff, const char* key)
{
	if (!_stricmp(key, "privatematchcustomclasses"))
	{
		key = "customClasses";
	}

	__asm
	{
		push key
		push stuff
		call getPlayerDataClassHook._Original
		add esp, 8h
	}
}

Hook::Call getMatchRulesDataHook;
DWORD getMatchRulesDataHookLoc = 0x4A210F;

bool lastWasAllowPerks = false;

void GetMatchRulesDataHook(int a1, void* a2, const char* a3)
{
	if (Scr_GetNumParam() == 2)
	{
		const char* first = Scr_GetString(0);

		if (!_stricmp(first, "commonOption"))
		{
			const char* field = Scr_GetString(1);

			if (!_stricmp(field, "hardcoreModeOn"))
			{
				if (!lastWasAllowPerks)
				{
					Scr_AddInt(0);
					return;
				}
			}

			if (!_stricmp(field, "allowPerks"))
			{
				lastWasAllowPerks = true;
			}
			else
			{
				lastWasAllowPerks = false;
			}
		}
	}

	__asm
	{
		push a3
		push a2
		push a1
		call getMatchRulesDataHook._Original
		add esp, 0Ch
	}
}

Hook::Call menuResponseHook;
DWORD menuResponseHookLoc = 0x47EF73;

void MenuResponseHookFunc(const char* string)
{
	if (!_stricmp(string, "endround"))
	{
		string = "cookie";
	}

	Scr_AddString(string);
}

typedef struct
{
	char mapname[64];
	char otherstuff[2756];
} arenaentry_t;

arenaentry_t* arenae = (arenaentry_t*)0x5780734;

Hook::Call mapRandHook;
DWORD mapRandHookLoc = 0x4E4333;

int MapRandHookFunc(int min, int max, int* seed)
{
	DWORD validateMapFunc = 0x456BA0;
	const char* mapname = NULL;
	bool isValidMap = false;
	int randomValue = 0;

	while (!isValidMap)
	{
		// get a random value
		__asm
		{
			push seed
			push max
			push min
			call mapRandHook._Original
			add esp, 0Ch

			mov randomValue, eax
		}

		// read this one's mapname
		mapname = arenae[randomValue].mapname;

		// and check if this is a valid map
		__asm
		{
			push 0
			push mapname
			call validateMapFunc
			add esp, 8

			mov isValidMap, al
		}

		// do some logging
		if (!isValidMap)
		{
			Com_Printf(0, "* rotation tried to go to invalid map %s\n", mapname);
		}
	}

	return randomValue;
}

typedef void*(__cdecl *UI_GetDvarInt_t)(const char * dvarName);
UI_GetDvarInt_t UI_GetDvarInt = (UI_GetDvarInt_t)0x56C390;

typedef void*(__cdecl *UI_GetDvarIntDS_t)(const char * dvarName);
UI_GetDvarIntDS_t UI_GetDvarIntDS = (UI_GetDvarIntDS_t)0x5314B0;

void* UI_GetDvarBoolHookFunc_DS(const char* dvarName)
{
	Log::Debug("UI_GetDvarBoolHookFunc_DS", "%s %d" , dvarName);
	return UI_GetDvarIntDS(dvarName);
}

void PatchIW5DS_Ranked()
{
	/*QCALL(0x495CF7, DvarGetIntHookFunc, QPATCH_CALL);
	QCALL(0x495DA2, DvarGetIntHookFunc, QPATCH_CALL);
	QCALL(0x495E83, DvarGetIntHookFunc, QPATCH_CALL);
	QCALL(0x495F75, DvarGetIntHookFunc, QPATCH_CALL);*/

	/*dvarGetIntHook.Initialize(0x531600, Hookkkk);
	dvarGetIntHook.InstallHook();*/


	//QCALL(0x505D9C, UI_GetDvarBoolHookFunc, QPATCH_CALL);
	//QCALL(0x505C3C, UI_GetDvarBoolHookFunc_DS, QPATCH_CALL);

	/*dvarGetIntHook.Initialize(0x531600, Hookkkk);
	dvarGetIntHook.InstallHook();*/


	/*
	dvarGetIntHook.Initialize(dvarGetIntHookLoc, DvarGetIntHookFunc);
	dvarGetIntHook.InstallHook();

	dvarGetHook.Initialize(dvarGetHookLoc, DvarGetHookFunc);
	dvarGetHook.InstallHook();

	getPlayerDataClassHook.Initialize(getPlayerDataClassHookLoc, GetPlayerDataClassHook);
	getPlayerDataClassHook.InstallHook();

	getMatchRulesDataHook.Initialize(getMatchRulesDataHookLoc, GetMatchRulesDataHook);
	getMatchRulesDataHook.InstallHook();

	menuResponseHook.Initialize(menuResponseHookLoc, MenuResponseHookFunc);
	menuResponseHook.InstallHook();

	// skip 'map rotation treshold'
	*(BYTE*)0x4E41A6 = 0xEB;

	mapRandHook.Initialize(mapRandHookLoc, MapRandHookFunc);
	mapRandHook.InstallHook();*/
}


bool Menu_IsMenuOpenAndVisible(int controller, const char* menu)
{
	bool retval = false;
	DWORD func = 0x4B4790;

	__asm
	{
		push menu
		push controller
		call func
		add esp, 8h
		mov retval, al
	}

	return retval;
}

dvar_t* g_compassForceDisplay;

Hook::Call uiGetDvarBoolHook;
DWORD uiGetDvarBoolHookLoc = 0x629E07;

void* UI_GetDvarIntHookFunc(const char * dvarName)
{
	Log::Error("UI_GetDvarIntHookFunc", dvarName);
	if (!_stricmp(dvarName, "xblive_privatematch"))
	{
		if (Menu_IsMenuOpenAndVisible(0, "changeclass_marines") || Menu_IsMenuOpenAndVisible(0, "changeclass_opfor"))
		{
			Log::Error("UI_GetDvarIntHookFunc", "xblive_privatematch faked!");
			return (void*)0x5B724A8;
		}

		//return (void*)0x669890;
	}

	return UI_GetDvarInt(dvarName);
}



void* UI_GetDvarBoolHookFunc(const char* dvarName)
{
	Log::Error("UI_GetDvarBoolHookFunc", dvarName);

	if (!_stricmp(dvarName, "g_compassshowenemies"))
	{
		return g_compassForceDisplay;
	}

	return UI_GetDvarInt(dvarName);
}

void PatchIW5MPRanked()
{
	g_compassForceDisplay = Dvar_RegisterBool("g_compassForceDisplay", 0, 4);

	QCALL(0x649CE7, UI_GetDvarBoolHookFunc, QPATCH_CALL);
	QCALL(0x649B87, UI_GetDvarIntHookFunc, QPATCH_CALL);
}