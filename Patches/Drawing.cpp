#include "..\stdafx.h"

void ListAllDvars();
void ListAllCommands();

Hook::Call drawDevStuffHook;
#pragma region Colors
vec4_t InsideColor2 = { 0.1f, 0.1f, 0.1f, 1.0f };
vec4_t InsideColor = { 0.1f, 0.1f, 0.1f, 1.0f };
vec4_t OutLineColor = { 0.60000002f, 0.60000002f, 0.60000002f, 1.0f };
vec4_t color3 = { 0.25f, 0.25f, 0.2f, 0.5f };
vec4_t color4 = { 0.11f, 0.11f, 0.11f, 0.9f };


vec4_t con_inputBoxColor = { 0.0f, 0.0f, 0.0f, 0.7f };

vec4_t WhileColor = { 1.0f, 1.0f, 1.0f, 1.0f };
vec4_t YellowColor = { 1.0f, 1.0f, 0.0f, 1.0f };
vec4_t RedColor = { 1.0f, 0.0f, 0.0f, 1.0f };
#pragma endregion

#pragma region Other
int R_GetScaledWidth(const char* text, float sizeX, void* font)
{
	if (!R_TextWidth)
	{
		return 0;
	}

	int normalWidth = R_TextWidth(text, 0x7FFFFFFF, (Font*)font);
	double scaledWidth = normalWidth * sizeX;
	return (int)scaledWidth;
}

bool CL_Ingame()
{
	dvar_t **cl_ingame = (dvar_t**)0x113D994;

	if ((*cl_ingame)->current.boolean == true)
		return true;
	else
		return false;
}
#pragma endregion

#pragma region ScreenSize
int getScreenHeight()
{
	if (Global::Game::Type == GameType::GAME_TYPE_SP)
		return *(int*)0xAA41E4;
	return *(int*)0x1091D64;
}

int getScreenWidth()
{
	if (Global::Game::Type == GameType::GAME_TYPE_SP)
		return *(int*)0xAA41E0;
	return *(int*)0x1091D60;
}
#pragma endregion

#pragma region DrawFps

int frames = 0;
int lastFrame = 0;
int lastUpdate = 0;
char fpsText[20];
float FpsColorYellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };

float FpsTextFont_size = 1.0f;
int FpsTextWidth = 0;
int FpsTextxOffset = 0;
int FpsTextyOffset = 30;

void* FpsFont;

void DrawFps()
{
	if (cg_drawfps->current.boolean == false)
		return;

	frames++;

	if (Com_Milliseconds() - lastUpdate > 40)
	{
		int fps = 1000.0 / ((double)(Com_Milliseconds() - lastUpdate) / (double)frames);

		snprintf(fpsText, 20, "%d FPS", fps);
		lastUpdate = Com_Milliseconds();
		frames = 0;
	}

	FpsTextWidth = R_GetScaledWidth(fpsText, FpsTextFont_size, FpsFont);
	FpsTextxOffset = getScreenWidth() - FpsTextWidth - 30;


	//draw fps
	R_AddCmdDrawText(fpsText, 0x7FFFFFFF, FpsFont, FpsTextxOffset, 50, FpsTextFont_size, FpsTextFont_size, 0.0f, FpsColorYellow, 0);
}
#pragma endregion

#pragma region DrawConsole
const char* branding = "> ";
float Console_a = 15.0f;// from left
float Console_b = 15.0f;// y
float Console_c = 2.0f;// outer line size  2
float Console_d = 15.0f;// from right
float Console_e = 30.0f;// console full height
float Console_f = 1.0f;// font size
float Console_w = getScreenWidth() - Console_a - Console_d;// Console width till to end
float Console_g = 7.5f; // Space between consoleTitle and consoleText;
float Console_h = 15.0f; // Space between first console end and
float Console_i = getScreenHeight() - 50; // the end of the second console
int Console_textWidth = 0;
void* ConsoleFont;
void* ConsoleMaterial;

void DrawConsole()
{
	if (!Drawing::Console::DrawConsoleInputBox)
		return;

	Console_a = 15.0f;// from left
	Console_b = 15.0f;// y
	Console_c = 2.0f;// outer line size  2
	Console_d = 15.0f;// from right
	Console_e = 30.0f;// console full height
	Console_f = 1.0f;// font size
	Console_w = getScreenWidth() - Console_a - Console_d;// Console width till to end
	Console_g = 7.5f; // Space between consoleTitle and consoleText;
	Console_h = 15.0f; // Space between first console end and
	Console_i = getScreenHeight() - 50; // the end of the second console
	Console_textWidth = R_GetScaledWidth(branding, 1.0f, ConsoleFont);

	vec4_t darkColor; // [esp+28h] [ebp-14h]
	darkColor[0] = con_inputBoxColor[0] * 0.5;
	darkColor[1] = con_inputBoxColor[1] * 0.5;
	darkColor[2] = con_inputBoxColor[2] * 0.5;
	darkColor[3] = con_inputBoxColor[3];

	//draw lil box
	R_AddCmdDrawStretchPicInternal(Console_a + 1, Console_b + 1, Console_w - 1, Console_e - 1, 0.0f, 0.0f, 0.0f, 0.0f, con_inputBoxColor, ConsoleMaterial); // Inside Rect
	R_AddCmdDrawStretchPicInternal(Console_a, Console_b, Console_w, Console_c, 0.0f, 0.0f, 0.0f, 0.0f, OutLineColor, ConsoleMaterial); // Top Left - top Right
	R_AddCmdDrawStretchPicInternal(Console_a, Console_b, Console_c, Console_e, 0.0f, 0.0f, 0.0f, 0.0f, OutLineColor, ConsoleMaterial); // Top Left - Lower Left
	R_AddCmdDrawStretchPicInternal(Console_a, Console_b + Console_e, Console_w + Console_c, Console_c, 0.0f, 0.0f, 0.0f, 0.0f, OutLineColor, ConsoleMaterial); // Top Right - Lower Right
	R_AddCmdDrawStretchPicInternal(Console_w + Console_d, Console_b, Console_c, Console_e + 2, 0.0f, 0.0f, 0.0f, 0.0f, OutLineColor, ConsoleMaterial);// Lower Left - Lower Right

																																				  //draw console title
	R_AddCmdDrawText(branding, 0x7FFFFFFF, ConsoleFont, 20.0f, 39.0f, 1.0f, 1.0f, 0, YellowColor, 0);

	//draw input text
	R_AddCmdDrawText(Drawing::Console::InputText.c_str(), 0x7FFFFFFF, ConsoleFont, 20.0f + Console_textWidth, 39.0f, 1.0f, 1.0f, 0, YellowColor, 0);

	if (Drawing::Console::DrawConsoleListBox)
	{
		//draw big box
		R_AddCmdDrawStretchPicInternal(Console_a, Console_b + Console_e + Console_h, Console_w, Console_i - Console_e - Console_h - Console_b - Console_b, 0.0, 0.0, 0.0, 0.0, color4, ConsoleMaterial);
		R_AddCmdDrawStretchPicInternal(Console_a, Console_b + Console_e + Console_h, Console_w, Console_c, 0.0, 0.0, 0.0, 0.0, OutLineColor, ConsoleMaterial);
		R_AddCmdDrawStretchPicInternal(Console_a, Console_b + Console_e + Console_h, Console_c, Console_i - Console_e - Console_h - Console_b - Console_b + Console_c, 0.0, 0.0, 0.0, 0.0, OutLineColor, ConsoleMaterial);
		R_AddCmdDrawStretchPicInternal(Console_w + Console_d, Console_b + Console_e + Console_h, Console_c, Console_i - Console_e - Console_h - Console_b - Console_b + Console_c, 0.0, 0.0, 0.0, 0.0, OutLineColor, ConsoleMaterial);
		R_AddCmdDrawStretchPicInternal(Console_a + Console_c, Console_i - Console_h - Console_a + Console_b, Console_w, Console_c, 0.0, 0.0, 0.0, 0.0, OutLineColor, ConsoleMaterial);

		if (!Drawing::Console::ConsoleLog.empty())
		{
			//erase old logs
			while (Drawing::Console::ConsoleLog.size() > 28)
			{
				Drawing::Console::ConsoleLog.erase(Drawing::Console::ConsoleLog.begin());
			}

			if (Drawing::Console::ConsoleLog.size() <= 28)
			{
				for (int s = 0; s<Drawing::Console::ConsoleLog.size(); s++)
				{
					Log::Debug("", "%d", Drawing::Console::ConsoleLog.size());
					R_AddCmdDrawText(Drawing::Console::ConsoleLog[s], 0x7FFFFFFF, ConsoleFont, 10.0f + Console_textWidth, Console_b + Console_e + Console_h + 20.0f + (15.0f * s), 1.0f, 1.0f, 0, WhileColor, 0);
				}
			}
		}
	}

	//draw sugest
	if (Drawing::Console::SuggestedDvarsList.size() == 0)
		return;

	if (Drawing::Console::SuggestedDvarsList.size() < 29)
	{
		R_AddCmdDrawStretchPicInternal(Console_a, Console_b + Console_e + Console_h, Console_w, (Drawing::Console::SuggestedDvarsList.size() + 1) * 15.0f, 0.0, 0.0, 0.0, 0.0, color4, ConsoleMaterial);
		R_AddCmdDrawStretchPicInternal(Console_a, Console_b + Console_e + Console_h, Console_w, Console_c, 0.0, 0.0, 0.0, 0.0, OutLineColor, ConsoleMaterial);
		R_AddCmdDrawStretchPicInternal(Console_a, Console_b + Console_e + Console_h, Console_c, (Drawing::Console::SuggestedDvarsList.size() + 1) * 15.0f, 0.0, 0.0, 0.0, 0.0, OutLineColor, ConsoleMaterial);
		R_AddCmdDrawStretchPicInternal(Console_w + Console_d, Console_b + Console_e + Console_h, Console_c, (Drawing::Console::SuggestedDvarsList.size() + 1) * 15.0f, 0.0, 0.0, 0.0, 0.0, OutLineColor, ConsoleMaterial);
		R_AddCmdDrawStretchPicInternal(Console_a, (Drawing::Console::SuggestedDvarsList.size() + 1) * 15.0f + 60, Console_w + 2, Console_c, 0.0, 0.0, 0.0, 0.0, OutLineColor, ConsoleMaterial);

		for (int n = 0; n<Drawing::Console::SuggestedDvarsList.size(); n++)
		{
			R_AddCmdDrawText(Drawing::Console::SuggestedDvarsList[n], 0x7FFFFFFF, ConsoleFont, 10.0f + Console_textWidth, Console_b + Console_e + Console_h + 20.0f + (15.0f * n), 1.0f, 1.0f, 0, WhileColor, 0);
		}
	}
}

#pragma endregion

#pragma region DrawSubtitle

float Subtitle_a = 0;// from left
float Subtitle_b = 15.0f;// y
float Subtitle_c = 2.0f;// outer line size  2
float Subtitle_d = 0;// from right
float Subtitle_e = 30.0f;// console full height
float Subtitle_w = 0;

void* NormalFont;
float SubtitleTextFont_size = 0.7f;
int SubtitleTextWidth = 0;
float SubtitleTextxOffset = 0;
int SubtitleTextyOffset = 30;
float SubtitleColorYellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
const char* SubtitleText = "^2Dishab oomadam khoonatoon naboodi! rastesho begoo koja rafte boodi!!!!! ^3PlusIW5 ^7Alpha";

void DrawSubtitle()
{
	if (Global::Game::Type == GameType::GAME_TYPE_SP)
		return;

	if (CL_Ingame())
		return;

	Subtitle_b = getScreenHeight() - 50;
	Subtitle_w = getScreenWidth();// subtitle width

	R_AddCmdDrawStretchPicInternal(Subtitle_a, Subtitle_b, Subtitle_w, Subtitle_e, 0.0f, 0.0f, 0.5f, 0.5f, color4, ConsoleMaterial); // Inside Rect
	R_AddCmdDrawStretchPicInternal(Subtitle_a, Subtitle_b, Subtitle_w, Subtitle_c, 0.0f, 0.0f, 0.5f, 0.5f, OutLineColor, ConsoleMaterial); // Top Left - top Right
	//R_AddCmdDrawStretchPicInternal(Subtitle_w + Subtitle_d, Subtitle_b, Subtitle_c, Subtitle_e, 0.0f, 0.0f, 0.5f, 0.5f, OutLineColor, ConsoleMaterial);// Lower Left - Lower Right
	R_AddCmdDrawStretchPicInternal(Subtitle_w + Subtitle_d, Subtitle_b, Subtitle_c, Subtitle_e, 0.0f, 0.0f, 0.5f, 0.5f, OutLineColor, ConsoleMaterial);// Lower Left - Lower Right

	SubtitleTextWidth = R_GetScaledWidth(SubtitleText, SubtitleTextFont_size, NormalFont);



	if ((SubtitleTextWidth + SubtitleTextxOffset) <= 0)
		SubtitleTextxOffset = getScreenWidth();
	else
		SubtitleTextxOffset = SubtitleTextxOffset - 0.8;



	//draw Subtitle
	R_AddCmdDrawText(SubtitleText, 0x7FFFFFFF, NormalFont, SubtitleTextxOffset, Subtitle_b + 25, SubtitleTextFont_size, SubtitleTextFont_size, 0.0f, SubtitleColorYellow, 0);
}
#pragma endregion




bool isMaterialsLoaded = false;

void LoadMaterials()
{
	ConsoleMaterial = Material_RegisterHandle("white");
	ConsoleFont = R_RegisterFont("fonts/consoleFont");
	FpsFont = R_RegisterFont("fonts/smalldevfont");
	NormalFont = R_RegisterFont("fonts/normalfont");
	isMaterialsLoaded = true;
}



void DrawStuff()
{
	//(*cg_drawfps)->flags = DVAR_FLAG_SAVED;

	if (!isMaterialsLoaded)
	{
		LoadMaterials();//one time

		ListAllDvars();
		ListAllCommands();
	}

	if (Drawing::Console::DvarsList.size() == 0)//retry until list all
		ListAllDvars();

	if (Drawing::Console::CommandsList.size() == 0)
		ListAllCommands();

	DrawSubtitle();
	DrawConsole();
	DrawFps();
}

void __declspec(naked) DrawDevStuffHookStub()
{
	__asm
	{
		call DrawStuff
		jmp drawDevStuffHook._Original
	}
}

void Vid_Restart()
{
	if (Global::Game::Type == GameType::GAME_TYPE_SP)
		((void(*)())0x41F060)();
	else if (Global::Game::Type == GameType::GAME_TYPE_MP)
		((void(*)())0x43A310)();

	Log::Debug("Vid_Restart", "Loading Materials");
	LoadMaterials();
}

void Patch_IW5FPS()
{
	/*Com_Milliseconds = (Com_Milliseconds_t)0x5181C0;
	R_AddCmdDrawText = (R_AddCmdDrawText_t)0x49C200;
	R_TextWidth = (R_TextWidth_t)0x43C190;
	R_RegisterFont = (R_RegisterFont_t)0x485440;
	Material_RegisterHandle = (Material_RegisterHandle_t)0x55FFC0;
	R_AddCmdDrawStretchPicInternal = (R_AddCmdDrawStretchPicInternal_t)0x57C7F0;
	UI_DrawHandlePic = (UI_DrawHandlePic_t)0x57D450;
	ScrPlace_GetActivePlacement = (ScrPlace_GetActivePlacement_t)0x522880;
	Key_IsCatcherActive = (Key_IsCatcherActive_t)0x44E9A0;
	Key_SetCatcher = (Key_SetCatcher_t)0x42D540;
	Key_RemoveCatcher = (Key_RemoveCatcher_t)0x4D9BA0;
	Key_IsDown = (Key_IsDown_t)0x4E9350;
	CL_KeyEvent = (CL_KeyEvent_t)0x46A420;*/

	
	if (Global::Game::Type == GameType::GAME_TYPE_SP)
	{
		//*(BYTE*)0x682665 = 0xEB;

		QCALL(0x608A96, Vid_Restart, QPATCH_CALL);

		drawDevStuffHook.Initialize(0x57F7E4, DrawDevStuffHookStub);
		drawDevStuffHook.InstallHook();
	}
	else if (Global::Game::Type == GameType::GAME_TYPE_MP)
	{
		QCALL(0x5798F2, Vid_Restart, QPATCH_CALL);

		drawDevStuffHook.Initialize(0x5D55BE, DrawDevStuffHookStub);
		drawDevStuffHook.InstallHook();
	}
}