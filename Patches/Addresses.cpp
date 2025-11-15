#include "..\stdafx.h"

void FindAddresses()
{
	//MP 1.9.461 addresses
	//Com_Milliseconds = (Com_Milliseconds_t)0x5181C0;
	//R_AddCmdDrawText = (R_AddCmdDrawText_t)0x49C200;
	//R_TextWidth = (R_TextWidth_t)0x43C190;
	//R_RegisterFont = (R_RegisterFont_t)0x485440;
	//Material_RegisterHandle = (Material_RegisterHandle_t)0x55FFC0;
	//R_AddCmdDrawStretchPicInternal = (R_AddCmdDrawStretchPicInternal_t)0x57C7F0;
	//UI_DrawHandlePic = (UI_DrawHandlePic_t)0x57D450;
	//ScrPlace_GetActivePlacement = (ScrPlace_GetActivePlacement_t)0x522880;
	//Key_IsCatcherActive = (Key_IsCatcherActive_t)0x44E9A0;
	//Key_SetCatcher = (Key_SetCatcher_t)0x42D540;
	//Key_RemoveCatcher = (Key_RemoveCatcher_t)0x4D9BA0;
	//Key_IsDown = (Key_IsDown_t)0x4E9350;
	//CL_KeyEvent = (CL_KeyEvent_t)0x46A420;

	DWORD Com_MillisecondsAddr = (Findpattern(Pattern::Textsegment, "CC ? ? ? ? ? ? ? 56 ? ? ? ? ? ? 75 1E") + 1);
	DWORD R_AddCmdDrawTextAddr = Findpattern(Pattern::Textsegment, "8B 44 24 04 80 38 00 0F 84 32 01 00 00 8D 50 01 8A 08 40 84 C9 75 F9 8B 0D ? ? ? ? 55 8B 2D");

	Com_Milliseconds = (Com_Milliseconds_t)Com_MillisecondsAddr;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)R_AddCmdDrawTextAddr;
	R_TextWidth = (R_TextWidth_t)Findpattern(Pattern::Textsegment, "51 53 57 33 DB 33 FF 39 5C 24 14 7F 08");
	R_RegisterFont = (R_RegisterFont_t)Findpattern(Pattern::Textsegment, "8B 44 24 04 6A 01 50 6A 18");
	Material_RegisterHandle = (Material_RegisterHandle_t)Findpattern(Pattern::Textsegment, "8B 44 24 04 80 38 00 75 06");
	R_AddCmdDrawStretchPicInternal = (R_AddCmdDrawStretchPicInternal_t)(Findpattern(Pattern::Textsegment, "CC 56 8B 74 24 2C 85 F6 75 06") + 1);
	UI_DrawHandlePic = (UI_DrawHandlePic_t)Findpattern(Pattern::Textsegment, "D9 EE 83 EC 0C D9 44 24 1C D8 D1 DF E0 F6 C4 05");
	ScrPlace_GetActivePlacement = (ScrPlace_GetActivePlacement_t)(Findpattern(Pattern::Textsegment, "CC A1 88 DC B4 00 83 F8 03 77 0D") + 1);

	Key_IsDown = (Key_IsDown_t)Findpattern(Pattern::Textsegment, "8B ? 24 08 83 ? FF 75 03 33 C0 C3 8B ? 24 04");
	CL_KeyEvent = (CL_KeyEvent_t)Findpattern(Pattern::Textsegment, "51 53 55 8B 6C 24 10 56 69 ED");
	Cbuf_AddText = (Cbuf_AddText_t)Findpattern(Pattern::Textsegment, "6A 1F E8 ???? 8B 44 24 0C");///shitttttttttttttttt   access violation reading
	Com_Error = (Com_Error_t)Findpattern(Pattern::Textsegment, "CC E8 2B 6B 07 00 84 C0 74 14 E8");

	if (Global::Game::Type == GameType::GAME_TYPE_SP) 
	{
		Dvar_RegisterBool = (Dvar_RegisterBool_t)0x4914D0;
		Cmd_AddCommand = (Cmd_AddCommand_t)0x558820;
		cmd_functions = (cmd_functions_s*)0x17507D8;
		Dvar_ForEachName = (Dvar_ForEachName_t)0x536720;
		Con_Print = (Con_Print_t)0x00;//no ingame console
	}
	else if (Global::Game::Type == GameType::GAME_TYPE_MP)
	{
		cmd_functions = (cmd_functions_s*)0x1D72DD8;
		Dvar_ForEachName = (Dvar_ForEachName_t)0x568A50;
		Con_Print = (Con_Print_t)0x510080;
		//cg_drawfps = (dvar_t**)0x9D7DF0;
	}
	else if (Global::Game::Type == GameType::GAME_TYPE_DEDI)
	{
		Cbuf_AddText = (Cbuf_AddText_t)0x4CB7A0;
	}
}