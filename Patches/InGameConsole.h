#pragma once

class Drawing
{
public:
	class Console
	{
	public:
		static bool isReadytoListCommands;
		static bool Catching;
		static std::string InputText;
		static dvar_t** sortedDvars;
		static bool DrawConsoleInputBox;
		static bool DrawConsoleListBox;
		static std::vector <const char*> DvarsList;
		static std::vector <const char*> CommandsList;
		static std::vector <const char*> SuggestedDvarsList;
		static std::vector <const char*> ConsoleLog;
	};
};