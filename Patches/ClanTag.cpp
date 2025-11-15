#include "..\stdafx.h"

typedef struct clientState_s
{
	int clientNum;
	char pad[120];
	int use_elite_clan_tag;
	int use_elite_clan_title;
	char elite_clan_title_text[24];
	int elite_clan_title_bg;
	char elite_clan_tag_text[8];
} clientState_t;

typedef struct
{
	char blah[209696];
	clientState_t clientState[18];
	char blaah[164];
} cgsnapshot_t;

cgsnapshot_t* cg_snapshots = (cgsnapshot_t*)0x9DF70C;

const char* AddClanTagToName(int* client, const char* oldName)
{
	static char tagName[32];
	int clientNum = *client;

	// find the clientState_t
	for (int i = 0; i < 18; i++)
	{
		clientState_t* state = &cg_snapshots[0].clientState[i];

		if (state->clientNum == clientNum)
		{
			if (state->use_elite_clan_tag)
			{
				sprintf_s(tagName, "^3[%s^7]%s", state->elite_clan_tag_text, oldName);

				return tagName;
			}
		}
	}

	return oldName;
}

void __declspec(naked) DrawListString_name()
{
	__asm
	{
		push edi // intended name to draw
		push ebp // pointer to clientnum
		call AddClanTagToName
		add esp, 8h

		mov edi, eax // returned pointer

		push 5B9580h // DrawListString itself
		retn
	}
}

void Patch_IW5ClanTag()
{
	QCALL(0x5B9E7E, DrawListString_name, QPATCH_CALL);
	QCALL(0x5B9EB8, DrawListString_name, QPATCH_CALL);
}