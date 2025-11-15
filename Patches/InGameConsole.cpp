#include "..\stdafx.h"

bool Drawing::Console::DrawConsoleInputBox = false;
bool Drawing::Console::DrawConsoleListBox = false;
bool Drawing::Console::Catching = false;
bool Drawing::Console::isReadytoListCommands = false;
std::string Drawing::Console::InputText = "";
dvar_t** Drawing::Console::sortedDvars;
std::vector <const char*> Drawing::Console::DvarsList;
std::vector <const char*> Drawing::Console::CommandsList;
std::vector <const char*> Drawing::Console::SuggestedDvarsList;
std::vector <const char*> Drawing::Console::ConsoleLog;

/*bool catching = false;

std::string console_input = "";

dvar_t** sortedDvars;*/

void FindSuggestionDvars(const char *word);
const char *GetDvarWithValue(dvar_t *dvar);

void InitConsoleWithLog()
{
	if (Drawing::Console::DrawConsoleInputBox)
	{
		Drawing::Console::DrawConsoleInputBox = false;
		Drawing::Console::DrawConsoleListBox = false;
		Drawing::Console::Catching = false;
	}
	else
	{
		Drawing::Console::DrawConsoleInputBox = true;
		Drawing::Console::DrawConsoleListBox = true;
		Drawing::Console::Catching = true;
	}
}

void InitConsole()
{
	if (Drawing::Console::DrawConsoleInputBox)
	{
		Drawing::Console::DrawConsoleInputBox = false;
		Drawing::Console::DrawConsoleListBox = false;
		Drawing::Console::Catching = false;
	}
	else
	{
		Drawing::Console::DrawConsoleInputBox = true;
		Drawing::Console::DrawConsoleListBox = false;
		Drawing::Console::Catching = true;
	}
}

std::string GetClipboardText()
{
	HANDLE h;

	if (!OpenClipboard(NULL))
		Log::Error("GetClipboardText", "Can't open clipboard");

	h = GetClipboardData(CF_TEXT);

	// Save text in a string class instance
	std::string text((const char*)h);

	// Release the lock
	GlobalUnlock(h);

	// Release the clipboard
	CloseClipboard();

	return text;
}


char ConverttoUppercase(char letter)
{
	char result = 0;

	switch (letter)
	{
		case 91:
			result = 123;//[ => {
			break;
		case 93:
			result = 125;//] => }
			break;
		case 92:
			result = 124;//\ => |
			break;
		case 59:
			result = 58;//; => :
			break;
		case 39:
			result = 34;//' => "
			break;
		case 44:
			result = 60;//, => <
			break;
		case 46:
			result = 62;//. => >
			break;
		case 47:
			result = 63;/// => ?
			break;
		case 194:
			result = 63;/// => /    num pad
			break;
		case 96:
			result = 126;//` => ~
			break;
		case 49:
			result = 33;//1 => !
			break;
		case 50:
			result = 64;//2 => @
			break;
		case 51:
			result = 35;//3 => #
			break;
		case 52:
			result = 36;//4 => $
			break;
		case 53:
			result = 37;//5 => %
			break;
		case 54:
			result = 94;//6 => ^
			break;
		case 55:
			result = 38;//7 => &
			break;
		case 56:
			result = 42;//8 => *
			break;
		case 198:
			result = 42;//8 => *   num pad
			break;
		case 57:
			result = 40;//9 => (
			break;
		case 48:
			result = 41;//0 => :
			break;
		case 45:
			result = 95;//- => _
			break;
		case 61:
			result = 196;//= => +
		case 195:
			result = 95;//- => _   num pad
			break;
		case 196:
			result = 43;//+ => +   num pad

		case 97:
			result = 65;//a => A
			break;
		case 98:
			result = 66;//b => B
			break;
		case 99:
			result = 67;//c => C
			break;
		case 100:
			result = 68;//d => D
			break;
		case 101:
			result = 69;//e => E
			break;
		case 102:
			result = 70;//f => F
			break;
		case 103:
			result = 71;//g => G
			break;
		case 104:
			result = 72;//h => H
			break;
		case 105:
			result = 73;//i => I
			break;
		case 106:
			result = 74;//j => J
			break;
		case 107:
			result = 75;//k => K
			break;
		case 108:
			result = 76;//l => L
			break;
		case 109:
			result = 77;//m => M
			break;
		case 110:
			result = 78;//n => N
			break;
		case 111:
			result = 79;//o => O
			break;
		case 112:
			result = 80;//p => P
			break;
		case 113:
			result = 81;//q => Q
			break;
		case 114:
			result = 82;//r => R
			break;
		case 115:
			result = 83;//s => S
			break;
		case 116:
			result = 84;//t => T
			break;
		case 117:
			result = 85;//u => U
			break;
		case 118:
			result = 86;//v => V
			break;
		case 119:
			result = 87;//w => W
			break;
		case 120:
			result = 88;//x => X
			break;
		case 121:
			result = 89;//y => Y
			break;
		case 122:
			result = 90;//z => Z
			break;



		case 182:
			result = 55;//z => Z
			break;
		case 183:
			result = 56;//z => Z
			break;
		case 184:
			result = 57;//z => Z
			break;
		case 185:
			result = 52;//z => Z
			break;
		case 186:
			result = 53;//z => Z
			break;
		case 187:
			result = 54;//z => Z
			break;
		case 188:
			result = 49;//z => Z
			break;
		case 189:
			result = 50;//z => Z
			break;
		case 190:
			result = 51;//z => Z
			break;
		case 192:
			result = 48;//z => Z
			break;
		case 193:
			result = 46;//z => Z
			break;
		default:
			result = 0;
			break;
	}

	return result;
}

void CL_KeyEventHook(int a1, int key, int down, int a4)
{
	if (Global::Game::Type == GameType::GAME_TYPE_SP)
		if (down)
		{
			if (key == 96 || key == 126)
			{
				if (Key_IsDown(0, 160))
					InitConsoleWithLog();
				else
					InitConsole();

				goto LETITGO;
			}
		}

	if (Drawing::Console::Catching)
	{
		// enter pressed
		if (!down)
		{
			if (key == 191 || key == 13)//num pad and key pad enter
			{
				if (!Drawing::Console::InputText.empty())
				{
					Drawing::Console::ConsoleLog.push_back(Drawing::Console::InputText.c_str());//log box

					Drawing::Console::InputText.append("\n");

					Log::Error("", Drawing::Console::InputText.c_str());

					Cbuf_AddText(0, va("%s", &Drawing::Console::InputText[0]));

					//Cmd_ExecuteSingleCommand(0, 0, Drawing::Console::InputText.c_str());
					Drawing::Console::InputText.clear();

					Drawing::Console::DrawConsoleInputBox = false;
					Drawing::Console::DrawConsoleListBox = false;
					Drawing::Console::Catching = false;
				}

				return;
			}
		}

		// backspace
		if (down && key == 127)
		{
			if (!Drawing::Console::InputText.empty())
			{
				Drawing::Console::InputText = Drawing::Console::InputText.size() > 1 ? Drawing::Console::InputText.substr(0, Drawing::Console::InputText.size() - 1) : "";
				FindSuggestionDvars(Drawing::Console::InputText.c_str());
			}
			goto LETITGO;
		}

		// tab logic
		if (down && key == 9)
		{
			if (Drawing::Console::DvarsList.size() > 0)
			{
				const char *dvarwithvalue = Drawing::Console::SuggestedDvarsList[0];
				Drawing::Console::InputText = dvarwithvalue;
			}
			return;
		}

		// space
		if (down && key == 32)
		{
			/*if (Drawing::Console::DvarsList.size() > 0)
			{*/
				Drawing::Console::InputText += ' ';
			//}
			return;
		}

		// letters, numbers and symbols
		if (down)
		{
			char upper = 0;

			if (key >= 97 && key < 126)//a to z
			{
				if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 || Key_IsDown(0, 160))//shift or caps
				{
					upper = ConverttoUppercase(key);
					if (upper == 0)
						Log::Error("CL_KeyEventHook", "Unable to Convert %i to UpperCase", key);
				}
				else
					upper = key;

				Drawing::Console::InputText += upper;
				FindSuggestionDvars(Drawing::Console::InputText.c_str());
				return;
			}
			/*else if (key >= 182 && key < 194)
			{
				upper = key;

				console_input += upper;
				FindSuggestionDvars(console_input.c_str());
				return;
			}*/


			//other supported numbers and symbols
			upper = ConverttoUppercase(key);

			if (upper > 0)
			{
				if (!Key_IsDown(0, 160))
					upper = key;

				Drawing::Console::InputText += upper;
				FindSuggestionDvars(Drawing::Console::InputText.c_str());
				return;
			}
			else
				Log::Error("CL_KeyEventHook", "Unable to Convert %i to UpperCase", key);
		}
	}



LETITGO:
	CL_KeyEvent(a1, key, down, a4);
}

void Patch_IW5Console()
{
	if (Global::Game::Type == GameType::GAME_TYPE_SP)
		QCALL(0x54BA30, CL_KeyEventHook, QPATCH_CALL);
	else if (Global::Game::Type == GameType::GAME_TYPE_MP)
	{
		QCALL(0x46A855, InitConsoleWithLog, QPATCH_CALL);
		QCALL(0x46A580, InitConsole, QPATCH_JUMP);
		QCALL(0x4C5820, CL_KeyEventHook, QPATCH_CALL);
	}
}