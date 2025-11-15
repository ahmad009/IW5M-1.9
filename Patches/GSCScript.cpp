#include "..\Stdafx.h"

typedef signed int(__cdecl * AssetExist_t)(int a1, int a2);
AssetExist_t AssetExist = (AssetExist_t)0x44EB10;

typedef int(__cdecl * AssetRead_t)(int a1, char *a2, int a3);
AssetRead_t AssetRead = (AssetRead_t)0x450F10;


/*dvar_t* Dvar_RegisterBool(const char* name, int default, int flags)
{

	dvar_t* retval = NULL;
	DWORD registerBool = 0x433060;

	__asm
	{
		push flags
		push default
		push name
		call registerBool
		mov retval, eax
		add esp, 0Ch
	}

	return retval;
}*/


void Patch_IW5LoadRawFiles()
{
	
}