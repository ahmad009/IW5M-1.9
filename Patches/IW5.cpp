// ==========================================================
// alterIWnet project
// 
// Component: aiw_client
// Sub-component: steam_api
// Purpose: IW5 function definitions
//
// Initial author: NTAuthority
// Started: 2012-01-29
// ==========================================================

#include "..\Stdafx.h"


//MSG_Init_t MSG_Init = (MSG_Init_t)0;
DB_FindXAssetHeader_t DB_FindXAssetHeader = (DB_FindXAssetHeader_t)0;
Dvar_SetCommand_t Dvar_SetCommand = (Dvar_SetCommand_t)0x57AB20;//1.9 client
Cbuf_AddText_t Cbuf_AddText = (Cbuf_AddText_t)0x454270;//1.9 client
FS_ReadFile_t FS_ReadFile = (FS_ReadFile_t)0x526AD0; //1.9 Dedi
Con_Print_t Con_Print = (Con_Print_t)0x00;//1.9 client
Dvar_FindVar_t Dvar_FindVar = (Dvar_FindVar_t)0;
Com_Error_t Com_Error = (Com_Error_t)0;
Scr_AddString_t Scr_AddString = (Scr_AddString_t)0x4F11D0;// 1.9 Dedi
Scr_AddInt_t Scr_AddInt = (Scr_AddInt_t)0x4F1020;// 1.9 Dedi
Scr_AddFloat_t Scr_AddFloat = (Scr_AddFloat_t)0x4F1070;// 1.9 Dedi
Scr_AddEntityNum_t Scr_AddEntityNum = (Scr_AddEntityNum_t)0x4DE360;//
Scr_AddVector_t Scr_AddVector = (Scr_AddVector_t)0x4F12F0;// 1.9 Dedi
SL_GetString_t SL_GetString = (SL_GetString_t)0x4F11D0;//  1.9 Dedi
Scr_NotifyNum_t Scr_NotifyNum = (Scr_NotifyNum_t)0x4DD3B0;//
Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x410A00;//1.9 client558820
//Scr_GetNumParam_t Scr_GetNumParam = (Scr_GetNumParam_t)0x4DE1B0;// no need
Scr_GetString_t Scr_GetString = (Scr_GetString_t)0x4DDB70;//
SV_GameSendServerCommand_t SV_GameSendServerCommand = (SV_GameSendServerCommand_t)0x4E8B80;//4FB610
Cmd_TokenizeString_t Cmd_TokenizeString = (Cmd_TokenizeString_t)0x4BF680;//4CD400
Cmd_EndTokenizedString_t Cmd_EndTokenizedString = (Cmd_EndTokenizedString_t)0x4BF6B0;//
Scr_NotifyLevel_t Scr_NotifyLevel = (Scr_NotifyLevel_t)0x4DD3E0;//
FS_Printf_t FS_Printf = (FS_Printf_t)0x438120;
//Key_KeynumToString_t Key_KeynumToString = (Key_KeynumToString_t)0x4CCFF0;
Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand = (Cmd_ExecuteSingleCommand_t)0x51B8E0;
Com_Milliseconds_t Com_Milliseconds = (Com_Milliseconds_t)0x00;
R_AddCmdDrawText_t R_AddCmdDrawText = (R_AddCmdDrawText_t)0x00;
R_TextWidth_t R_TextWidth = (R_TextWidth_t)0x00;
R_RegisterFont_t R_RegisterFont = (R_RegisterFont_t)0x00;
Material_RegisterHandle_t Material_RegisterHandle = (Material_RegisterHandle_t)0x00;
R_AddCmdDrawStretchPicInternal_t R_AddCmdDrawStretchPicInternal = (R_AddCmdDrawStretchPicInternal_t)0x00;
UI_DrawHandlePic_t UI_DrawHandlePic = (UI_DrawHandlePic_t)0x00;
ScrPlace_GetActivePlacement_t ScrPlace_GetActivePlacement = (ScrPlace_GetActivePlacement_t)0x00;
UI_OpenMenu_t UI_OpenMenu = (UI_OpenMenu_t)0x406CD0;
SetErrorTextMessage_t SetErrorTextMessage = (SetErrorTextMessage_t)0x440330;
Dvar_RegisterBool_t Dvar_RegisterBool = (Dvar_RegisterBool_t)0x53DE20;
Dvar_RegisterInt_t Dvar_RegisterInt = (Dvar_RegisterInt_t)0x4AFB00;

Key_IsCatcherActive_t Key_IsCatcherActive = (Key_IsCatcherActive_t)0x00;
Key_SetCatcher_t Key_SetCatcher = (Key_SetCatcher_t)0x00;
Key_RemoveCatcher_t Key_RemoveCatcher = (Key_RemoveCatcher_t)0x00;
CL_KeyEvent_t CL_KeyEvent = (CL_KeyEvent_t)0x00;
Key_IsDown_t Key_IsDown = (Key_IsDown_t)0x00;
Dvar_ForEachName_t Dvar_ForEachName = (Dvar_ForEachName_t)0x00;

DWORD* ConsolePrintAddr = (DWORD*)0x5275F0;
dvar_t *cg_drawfps;
//dvar_t **cg_drawfps = (dvar_t**)0x00;

cmd_functions_s* cmd_functions = (cmd_functions_s*)0x00;//sp = 17507D8   mp = 0x1D72DD8

bool g_isDedicated;

DWORD* cmd_id = (DWORD*)0;
DWORD* cmd_argc = (DWORD*)0;
DWORD** cmd_argv = (DWORD**)0;

DWORD* cmd_id_sv = (DWORD*)0x1B3EB98;
DWORD* cmd_argc_sv = (DWORD*)0x1B3EBDC;
DWORD** cmd_argv_sv = (DWORD**)0x1B3EBFC;

DWORD* scr_numParam = (DWORD*)0x1F58098;


int Scr_GetNumParam()
{
	return *(DWORD*)0x1F58098;
}


gentity_t* g_entities = (gentity_t*)0x18FBB28;

typedef void ExConsolePrint(const char* msg);
ExConsolePrint* exprint;

void Console_Printf(const char* data)
{
	/*static char buffer[32768];
	va_list ap;
	va_start(ap, data);
	_vsnprintf(buffer, sizeof(buffer), data, ap);
	va_end(ap);
	*/

	exprint(va("%s\n", data));
}

void Com_Printf(const char* format, ...)//int channel, 
{
	static char buffer[32768];
	va_list ap;
	va_start(ap, format);
	_vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);

	buffer[sizeof(buffer) - 1] = '\0';

	if (g_isDedicated)
	{
		__asm
		{
			push offset buffer
			//push channel
			mov eax, 4DA330h // Com_Print
			call eax
			add esp, 8h
		}
	}
	else
	{
		Console_Printf(buffer);
	}
}

void OpenMenu(const char* name)
{
	__asm
	{
		push name
		push 59CAAC0h
		mov eax, 406CD0h
		call eax
		add esp, 8
	}
}

/*char* Auth_GetUsername()
{
	return "Ahmad009";
}

int Auth_GetUserID()
{
	return 1337;
}

char* Auth_GetSessionID()
{
	return "loooooooooooooooooooooooool";
}

char* Auth_GetCountry()
{
	return "IR";
}*/