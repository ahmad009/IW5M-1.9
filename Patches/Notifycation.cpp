#include "..\stdafx.h"

void IW5Notify(const char* title, const char* message)
{
	Dvar_SetCommand("com_errorTitle", title);
	Dvar_SetCommand("com_errorMessage", message);
	//OpenMenu("error_popmenu_party");
	//OpenMenu("error_popmenu_lobby");
	OpenMenu("error_popmenu_submenu");
	//OpenMenu("error_popmenu");
}


char* publisherVA(char *fmt, ...)
{
	char g_vaBuffer[4][4096];
	int g_vaNextBufferIndex = 0;

	Drawing::Console::isReadytoListCommands = true;

	IW5Notify("^3PlusIW5", "Welcome to New Plus^2MW3^7.\n\nDevelopers:\n^3Ahmad009^7 : Base Client and Dedicated Crack.\n^3Sa3id^7 : Anticheat and Security.\n^3Farzad123^7 : a Fucking Lazy Guy :|\n\n^7Special Thanks to:\nNTA\nMomo5502\nConvery");

	va_list ap;
	char *dest = &g_vaBuffer[g_vaNextBufferIndex][0];
	g_vaNextBufferIndex = (g_vaNextBufferIndex + 1) % 4;
	va_start(ap, fmt);
	vsprintf(dest, fmt, ap);
	va_end(ap);
	return dest;
}

void Patch_IW5StartupNotify()
{
	QCALL(0x531402, publisherVA, QPATCH_CALL);
}
