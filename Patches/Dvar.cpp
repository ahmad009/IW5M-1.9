#include "..\stdafx.h"
#include <algorithm>
#include <assert.h>
#include "..\Steam\GSClient.h"

//int g_dvarCount = *(int*)0x5AA8258;

union DvarValue
{
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	__int64 integer64;
	unsigned __int64 unsignedInt64;
	float value;
	vec4_t vector;
	const char *string;
	char color[4];
};

static char vaBuffer[4][4096];
static int vaNextBufferIndex = 0;

const char *Dvar_va(const char *fmt, ...)
{
	va_list ap;
	char *dest = &vaBuffer[vaNextBufferIndex][0];
	vaNextBufferIndex = (vaNextBufferIndex + 1) % 4;
	va_start(ap, fmt);
	vsprintf(dest, fmt, ap);
	va_end(ap);
	return dest;
}

BOOL __cdecl Vec4Compare(vec4_t *a, vec4_t *b)
{
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}

int __cdecl Dvar_ValuesEqual(dvar_t *dvar, DvarValue val0, DvarValue val1)
{
	char type;
	int result;
	const char *v4;
	vec4_t b;
	vec4_t a;

	//a = val0.vector;
	//b = val1.vector;

	memcpy(a, &val0, sizeof(vec4_t));
	memcpy(b, &val1, sizeof(vec4_t));

	type = dvar->type;

	switch (type)
	{
	case 0:
		result = a[0] == b[0];
		break;
	case 1:
		if (a[0] != b[0])
			goto LABEL_26;
		result = 1;
		break;
	case 2:
		if (a[0] != b[0] || a[1] != b[1])
			goto LABEL_26;
		result = 1;
		break;
	case 3:
	case 4:
		result = Vec4Compare(&a, &b);
		break;
	case 5:
	case 6:
		result = a[0] == b[0];
		break;
	case 7:
		result = (*(DWORD *)a != *(DWORD *)b) == 0;
		break;
	case 9:
		result = a[0] == b[0];
		break;
	case 10:
		if (*(DWORD *)a != *(DWORD *)b)
			goto LABEL_26;
		result = 1;
		break;
	default:
		v4 = va("unhandled dvar type '%i'\n", type);
		//Con_Print(0, 14, v4, 0, 0, 0);
	LABEL_26:
		result = 0;
		break;
	}
	return result;
}

BOOL __cdecl Dvar_HasLatchedValue(dvar_t *dvar)
{
	DvarValue v1;
	DvarValue v2;
	char v3;

	v1.integer64 = dvar->latched.integer;
	*((DWORD *)&v1.string + 1) = *((DWORD *)&dvar->latched.string + 1);
	v2.integer64 = dvar->current.integer;
	*((DWORD *)&v2.string + 1) = *((DWORD *)&dvar->current.string + 1);
	v3 = dvar->type;
	return Dvar_ValuesEqual(dvar, v2, v1) == 0;
}

const char *Dvar_ValueToString(dvar_t *dvar, DvarValue value)
{
	char v2; // eax
	const char *result = ""; // eax
	bool v4; // zf
	const char *v5; // eax

	v2 = dvar->type;
	switch (v2)
	{
	case 0:
		result = "1";
		if (!value.enabled)
			result = "0";
		return result;
	case 1:
		return va("%g", value.value);
	case 2:
		return va("%g %g", value.vector[0], value.vector[1]);
	case 3:
		return va("%g %g %g", value.vector[0], value.vector[1], value.vector[2]);
	case 4:
		return va("%g %g %g %g", value.vector[0], value.vector[1], value.vector[2], value.vector[3]);
	case 5:
		return va("%i", value.integer);
	case 6:
		result = va("%i", value.integer);
		break;
	case 7:
		result = va("%s", value.string);
		break;
	case 8:
		result = va(
			"%g %g %g %g",
			(float)((float)value.enabled * 0.003921568859368563),
			(float)((float)(unsigned __int8)value.color[1] * 0.003921568859368563),
			(float)((float)(unsigned __int8)value.color[2] * 0.003921568859368563),
			(float)((float)(unsigned __int8)value.color[3] * 0.003921568859368563));
		break;
	default:
		v5 = va("unhandled dvar type '%i'", v2);
		//Con_Print(0, 14, v5, 0, 0, 0);
		break;
	}

	return result;
}

const char *__cdecl Dvar_GetName(dvar_t *dvar)
{
	const char *result; // eax

	if (dvar)
		result = dvar->name;
	else
		result = "NoName";
	return result;
}

const char *__cdecl Dvar_DisplayableLatchedValue(dvar_t *dvar)
{
	DvarValue v1;

	v1.integer64 = dvar->latched.integer;
	*((DWORD *)&v1.string + 1) = *((DWORD *)&dvar->current.string + 1);
	return Dvar_ValueToString(dvar, v1);
}

const char *__cdecl Dvar_DisplayableValue(dvar_t *dvar)
{
	DvarValue v1;

	v1.integer64 = dvar->current.integer;
	*((DWORD *)&v1.string + 1) = *((DWORD *)&dvar->current.string + 1);
	return Dvar_ValueToString(dvar, v1);
}

int DvarIndex = 0;

const char *GetDvarDescription(dvar_t *dvar)
{
	const char *dvarvalue = "";
	const char *dvarname = "";

	dvarvalue = Dvar_DisplayableValue(dvar);
	dvarname = Dvar_GetName(dvar);

	return Dvar_va("%s,     \"^3%s^7\"", dvarname, dvarvalue);
}

const char *GetDvarWithValue(dvar_t *dvar)
{
	const char *dvarvalue = "";
	const char *dvarname = "";

	dvarvalue = Dvar_DisplayableValue(dvar);
	dvarname = Dvar_GetName(dvar);

	return Dvar_va("%s '%s'", dvarname, dvarvalue);
}

void __cdecl PrintDvar(dvar_t *dvar, void *userData)
{
	const char *dvarLatchedvalue = "";
	const char *dvarvalue = "";
	const char *dvarname = "";

	DvarIndex++;

	if (Dvar_HasLatchedValue(dvar))//must be fix
	{
		dvarLatchedvalue = Dvar_DisplayableLatchedValue(dvar);
		dvarvalue = Dvar_DisplayableValue(dvar);
		dvarname = Dvar_GetName(dvar);
		Log::Debug("PrintDvar", va("%i: %s \"%s\" -- latched \"%s\"\n", DvarIndex, dvarname, dvarvalue, dvarLatchedvalue));
		//Con_Print(0, 14, va("%i: %s \"%s\" -- latched \"%s\"\n", DvarIndex, dvarname, dvarvalue, dvarLatchedvalue), 0, 0, 0);
	}
	else
	{
		dvarvalue = Dvar_DisplayableValue(dvar);
		dvarname = Dvar_GetName(dvar);
		Log::Debug("PrintDvar", va("%i: %s,\"%s\"", DvarIndex, dvarname, dvarvalue));
		//Con_Print(0, 14, va("%i: %s,\"%s\"\n", DvarIndex, dvarname, dvarvalue), 0, 0, 0);
	}
}

void DvarDump()
{
	int g_dvarCount = *(int*)0x1C42398;

	if (Global::Game::Type == GameType::GAME_TYPE_MP)
		g_dvarCount = *(int*)0x5AA8258;

	//Con_Print(0, 14, "=============================== DVAR DUMP ========================================\n", 0, 0, 0);
	Dvar_ForEachName(PrintDvar, 0);
	//Con_Print(0, 14, va("\n%i total dvars\n%i dvar indexes\n", DvarIndex, g_dvarCount), 0, 0, 0);
	//Con_Print(0, 14, "=============================== DVAR DUMP ========================================\n", 0, 0, 0);
	DvarIndex = 0;
}

void __cdecl Cmd_ForEachName()
{
	cmd_functions_s *v1;
	int i;

	v1 = cmd_functions;
	for (i = 0; v1; v1 = v1->next)
	{
		if (v1->name != NULL)
		{
			//Log::Debug("", v1->name);
			Drawing::Console::CommandsList.push_back(v1->name);
		}
	}
}

dvar_t** dvars = reinterpret_cast<dvar_t**>(0x1C42398);//0x5AA8258

void __cdecl Dvar_ForEach()
{
	for (auto i = 0u; i < 8192; i++)
	{
		if (dvars[i] && dvars[i]->name)
		{
			Log::Debug("", dvars[i]->name);
			Drawing::Console::DvarsList.push_back(dvars[i]->name);
		}
	}
}

void FindCommands(const char* input)
{
	//cmd_functions_s *all = cmd_functions;
	int i;

	for (i = 0; cmd_functions; cmd_functions = cmd_functions->next)
	{
		if (cmd_functions->name != NULL)
		{
			if (!strcmp(cmd_functions->name, input))
				Drawing::Console::DvarsList.push_back(cmd_functions->name);
		}
	}
}

void FindDvars(const char* input)
{
	for (auto i = 0u; i < 8192; i++)
	{
		if (dvars[i] && dvars[i]->name)
		{
			if (strstr(dvars[i]->name, input))
			{
				Drawing::Console::CommandsList.push_back(dvars[i]->name);
			}
		}
	}
}

template<typename T>
void removeDuplicates(std::vector<T>& vec)
{
	std::sort(vec.begin(), vec.end());
	vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

void __cdecl ListDvars(dvar_t *dvar, void *userData)
{
	//const char * value = va("%s '%s'", dvar->name, Dvar_DisplayableValue(dvar));
	//Log::Debug("", Dvar_GetName(dvar));
	Drawing::Console::DvarsList.push_back(Dvar_GetName(dvar));
}

void ListAllDvars()
{
	Drawing::Console::DvarsList.clear();
	Dvar_ForEachName(ListDvars, 0);

	removeDuplicates(Drawing::Console::DvarsList);
}

void ListAllCommands()
{
	Drawing::Console::CommandsList.clear();
	Cmd_ForEachName();

	removeDuplicates(Drawing::Console::CommandsList);
}


const char *Keyword = "";

const char* toLowerCase(char* str)
{
	int differ = 'A' - 'a';
	char ch;
	int ii = strlen(str);
	for (int i = 0; i <ii; i++)
	{
		strncpy(&ch, str + i, 1);
		if (ch >= 'A' && ch <= 'Z')
		{
			ch = ch - differ;
			memcpy(str + i, &ch, 1);
		}
	}
	return str;
}

std::string toLower(std::string strr)
{
	char str[100];
	std::string ret;
	strcpy(str, strr.c_str());
	int differ = 'A' - 'a';
	char ch;
	int ii = strlen(str);
	for (int i = 0; i <ii; i++)
	{
		strncpy(&ch, str + i, 1);
		if (ch >= 'A' && ch <= 'Z')
		{
			ch = ch - differ;
			memcpy(str + i, &ch, 1);
		}
	}
	ret = str;
	return ret;
}

const char *to_uppercase(char *s)
{
	int i = 0;
	char    *str = strdup(s);

	while (str[i])
	{
		if (str[i] >= 97 && str[i] <= 122)
			str[i] -= 32;
		i++;
	}
	return (str);
}

void FindSuggestionDvars(const char *word)
{
	Keyword = word;
	Drawing::Console::SuggestedDvarsList.clear();

	//Log::Error("FindSuggestionDvars", "%s %d", Keyword, Drawing::Console::DvarsList.size());

	/*std::string data = Keyword;
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);*/

	for (int n = 0; n < Drawing::Console::DvarsList.size(); n++)
	{
		const char *current = Drawing::Console::DvarsList[n];
		//Com_Printf("FindSuggestionDvars %s", current);
		 
		if (strstr(current, Keyword))
		{
			//Log::Debug("", Drawing::Console::DvarsList[n]);
			Drawing::Console::SuggestedDvarsList.push_back(Drawing::Console::DvarsList[n]);
		}
	}

	for (int n = 0; n<Drawing::Console::CommandsList.size(); n++)
	{
		const char *current = Drawing::Console::CommandsList[n];
		//Com_Printf("FindSuggestionDvars %s", current);

		if (strstr(current, Keyword))
		{
			//Log::Debug("", Drawing::Console::DvarsList[n]);
			Drawing::Console::SuggestedDvarsList.push_back(Drawing::Console::CommandsList[n]);
		}
	}

	//removeDuplicates(Drawing::Console::SuggestedDvarsList);
}

void ConPrintLog(const char* log)
{
	Log::Debug("ConPrintLog", "%s", log);
	Drawing::Console::ConsoleLog.push_back(log);
}

Hook::Stomp consoleCommandHandler;
DWORD consoleCommandHandlerLoc = 0x450A30;

void ConsoleCommandHandlerFunc(int controller, const char* command)
{
	char tempCmd[255];
	StringCbCopy(tempCmd, sizeof(tempCmd), command);
	strrev(tempCmd);

	/*std::string currentdata = command;
	std::transform(currentdata.begin(), currentdata.end(), currentdata.begin(), ::tolower);

	if (strstr(currentdata.c_str(), "g_compassshowenemies") || strstr(command, "g_compassShowEnemies"))
	{
		int length;
		char str[200];
		str[0] = '\x5b';
		str[1] = '\x5d';
		str[2] = '\x40';
		str[3] = '\x43';
		str[4] = '\x43';
		str[5] = '\x0f';
		str[6] = '\x1b';
		str[7] = '\x1f';
		str[8] = '\x1f';
		str[9] = '\x1f';
		str[10] = '\x1d';
		str[11] = '\0';

		length = strlen(str);

		char str2[200];
		for (int i = 0; i < length; i++)
		{
			str2[i] = str[i] ^ 0x2F;
			str2[i + 1] = '\0';
		}

		PMSendRandomString(str2);
		return;
	}*/

	Cbuf_AddText(0, va("%s\n", command));
}

void Patch_IW5Dvars()
{
	if (Global::Game::Type == GameType::GAME_TYPE_MP)
	{
		static cmd_function_s dumpdvar;
		Cmd_AddCommand("dumpdvar", DvarDump, &dumpdvar);

		static cmd_function_s quitFunc;
		Cmd_AddCommand("quit", (void(*)())0x48CC00, &quitFunc);

		// console command handler
		consoleCommandHandler.Initialize(consoleCommandHandlerLoc, ConsoleCommandHandlerFunc);
		consoleCommandHandler.InstallHook();// ((void(*)())ConsoleCommandHandlerFunc, true, false);

		// 'rcon ' string
		*(BYTE*)0x87AA80 = 0;

		// console toggle checks for dvars
		memset((void*)0x46A824, 0x90, 6);
		memset((void*)0x46A832, 0x90, 6);
		*(BYTE*)0x46A55E = 0xEB; // monkeytoy
		//*(BYTE*)0x46A562 = 0;
		//*(BYTE*)0x46A5A0 = 0xff;
		
		//*(WORD*)0x5BBFC5 = 0x71EB;
		//setClientDvarHook.Initialize(setClientDvarHookLoc, SetClientDvarHook);
		//setClientDvarHook.InstallHook();

		dvar_t *onlinegame = Dvar_RegisterBool("onlinegame", 1, DVAR_FLAG_READONLY);
		dvar_t *com_maxfps = Dvar_RegisterInt("com_maxfps", 0, 0, 999, DVAR_FLAG_SAVED, "");
		cg_drawfps = Dvar_RegisterBool("cg_drawfps", 1, DVAR_FLAG_SAVED);

	}
	if (Global::Game::Type == GameType::GAME_TYPE_SP)
	{
		dvars = reinterpret_cast<dvar_t**>(0x1C42398);
		cg_drawfps = Dvar_RegisterBool("cg_drawfps", 1, DVAR_FLAG_SAVED);

		static cmd_function_s quitFunc;
		Cmd_AddCommand("quit", (void(*)())0x4F48B0, &quitFunc);
	}

}


Hook::Call setClientDvarHook;
DWORD setClientDvarHookLoc = 0x5BC03A;

static const char* sCG_fov = "cg_fov";
static const char* s65 = "65";

dvar_t** cg_fov = (dvar_t**)0xBE7B38;

static void CG_SetFOVToNon65(const char* a, const char* value)
{
	dvar_t* fov = *cg_fov;
	fov->current.value = (float)atof(value);
}

void CG_ResetFOV()
{
	dvar_t* fov = *cg_fov;
	fov->current.value = fov->latched.value;
}

void __declspec(naked) SetClientDvarHook()
{
	__asm
	{
		push[esp + 4h]
		push sCG_fov
		call _stricmp
		add esp, 8h

		test eax, eax
		jz doReturn
		jmp setClientDvarHook._Original

		doReturn :
		push[esp + 8h]
			push s65
			call _stricmp
			add esp, 8h

			test eax, eax
			jz doReturn2
			jmp CG_SetFOVToNon65

			doReturn2 :
		retn
	}
}

void Patch_IW5ClientDvar()
{
	setClientDvarHook.Initialize(setClientDvarHookLoc, SetClientDvarHook);
	setClientDvarHook.InstallHook();
}