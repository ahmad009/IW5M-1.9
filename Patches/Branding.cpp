#include "..\stdafx.h"

Hook::Stomp colorzzz;
DWORD colorzzzLoc = 0x4529E0;

void ClientCleanName(const char *in, char *out, int outSize)
{
	//const char* username = va("^3[^6PM^3]^4%s^7", Auth_GetUsername());
	const char* username = va("^:%s^7", Auth_GetUsername());
	strncpy(out, username, 32);
}

void EnableConsole()
{
	// Enable external console
	((void(*)())0x706FF0)();//6C0C10
	((void(*)())0x4A40B0)();//470AF0
}

LocalizeEntry* SEH_LocalizeAsset(int type, const char* ref, bool stuff)
{
	static LocalizeEntry entry;
	LocalizeEntry* dbEntry = (LocalizeEntry*)DB_FindXAssetHeader(type, ref, stuff);

	if (!dbEntry)
	{
		return dbEntry;
	}

	memcpy(&entry, dbEntry, sizeof(LocalizeEntry));

	//not now
	if (!strcmp(entry.name, "MENU_STORE_CAPS"))
	{
		entry.value = "^:NEW^7 STORE";
	}
	else if (!strcmp(entry.name, "PLATFORM_STORE_DESC"))
	{
		entry.value = "'Purchase' content packs.";
	}

	if (strcmp(entry.value, "VAC") == 0)
	{
		Log::Info("Localized", entry.name);
	}

	return &entry;
}


typedef struct
{
	char type;
	char pad[3];
	const char** folder;
	const char* file;
} StreamFile;

typedef struct
{
	char pad[20];
	StreamFile* stream;
	char pad2[76];
} snd_alias_t;

typedef struct
{
	const char* name;
	snd_alias_t* aliases;
	int numAliases;
} snd_alias_list_t;

snd_alias_list_t* aliases;

void replaceMusic(const char* assetName, const char* sound)
{
	if (!_stricmp(aliases->name, assetName))
	{
		if (strcmp(aliases->aliases->stream->file, sound))
		{
			aliases->aliases->stream->file = sound;
		}
	}
}

snd_alias_list_t* FindSoundAliasHookFunc(int type, const char* name)
{
	aliases = (snd_alias_list_t*)DB_FindXAssetHeader(type, name, false);

	replaceMusic("music_challenge_factory", "bt_rescue_end.mp3");
	replaceMusic("music_mainmenu", "bt_pragueescape_sniper.mp3");
	replaceMusic("music_opening", "bt_prague_tragic.mp3");

	return aliases;
}

void Maxclients()
{
	/**(BYTE*)0x4882BB = 24;//sv_maxclients
	*(BYTE*)0x488301 = 24;//ui_maxclients

	*(BYTE*)0x4FB084 = 24;//ui_maxclients
	*(BYTE*)0x4FB0D2 = 24;//sv_maxclients

	*(BYTE*)0x4FB9FD = 24;//sv_maxclients

	*(BYTE*)0x4FC17E = 24;//sv_maxclients*/

	*(BYTE*)0x436B35 = 0xEB;
	QNOP(0x436B1B, 5);
	QNOP(0x436BC3, 5);
	QNOP(0x436E5A, 5);
	QNOP(0x436FE7, 5);
	//QNOP(0x436B7F, 5);
	
}

void Patch_IW5Branding()
{
	if (Global::Game::Type == GameType::GAME_TYPE_SP)
	{
		//change menu musics
		QCALL(0x4DC99B, FindSoundAliasHookFunc, QPATCH_CALL);
	}
	else if (Global::Game::Type == GameType::GAME_TYPE_MP)
	{
		//for menu
		QCALL(0x5700F0, ClientCleanName, QPATCH_CALL);

		//for in game (needs to increase max name length)
		QCALL(0x5028EA, ClientCleanName, QPATCH_CALL);
		QCALL(0x5028B0, ClientCleanName, QPATCH_CALL);

		//new colors
		DWORD default_color_table[8];
		DWORD *color_table = (DWORD*)0x8D75F8;

		// NTA's W² Colors
		color_table[1] = RGB(255, 49, 49);//^1 red 
		color_table[2] = RGB(134, 192, 0);//^2 green
		color_table[3] = RGB(255, 173, 34);//^3 Yellow
		color_table[4] = RGB(0, 135, 193);//^4 Dark blue
		color_table[5] = RGB(32, 197, 255);//^5 Light blue
		color_table[6] = RGB(151, 80, 221);//^6 Pink

		for (int i = 0; i < 7 * 4; i++) ((float*)0x8B9678)[i] = ((BYTE*)color_table)[i] / 255.0f;

		//fs_basegame
		*(const char**)0x666671 = "PlusIW5";

		//version string
		*(DWORD*)0x4A54F1 = (DWORD)"PlusMW3\n1.9.461";
		strcpy((char*)0x8B17B4, "PlusIW5 Console");
		strcpy((char*)0x883DF0, "PlusIW5 Multiplayer");

		//remove splash image
		QNOP(0x540603, 5);

		//console window custom size
		*(WORD*)0x5582B1 = 850;
		*(WORD*)0x5582BD = 570;
		*(WORD*)0x558334 = -850;
		*(WORD*)0x55831F = -570;

		//log box and input textbox size and location
		*(WORD*)0x55841C = 840;
		*(WORD*)0x558421 = 545;
		*(WORD*)0x558449 = 840;
		*(WORD*)0x558444 = 470;

		EnableConsole();
		((LRESULT(__cdecl*)())0x4A40B0)();

		//localized strings
		QCALL(0x4B3443, SEH_LocalizeAsset, QPATCH_CALL);
		QCALL(0x48E8B9, SEH_LocalizeAsset, QPATCH_CALL);
		QCALL(0x53DE94, SEH_LocalizeAsset, QPATCH_CALL);
	}
	else
	{
		//console window custom size
		*(WORD*)0x53C477 = 850;
		*(WORD*)0x53C483 = 570;
		*(WORD*)0x53C4FB = -850;
		*(WORD*)0x53C4E6 = -570;

		//log box and input textbox size and location
		*(WORD*)0x53C5E7 = 840;
		*(WORD*)0x53C5EC = 545;
		*(WORD*)0x53C611 = 840;
		*(WORD*)0x53C60C = 470;

		//Maxclients();

		// revision number
		*(const char**)0x4DAAD7 = "Welcome to PlusMW3 1.9.461 Beta v1 Build 1.\nVisit http://plusmaster.pro/ for more information\n";
	}
}