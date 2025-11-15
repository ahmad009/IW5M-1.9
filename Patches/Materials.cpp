#include "..\stdafx.h"

typedef signed int(__cdecl * AssetExist)(int a1, int a2);
AssetExist exist = (AssetExist)0x47F970;

typedef int(__cdecl * AssetExist_Read)(int a1, char *a2, int a3);
AssetExist_Read Read = (AssetExist_Read)0x40B220;

Hook::Stomp CustomFont;
Hook::Stomp CustomMaterial;
Hook::Stomp CustomMaterial2;
Hook::Stomp CustomMaterial3;

/*
Loading fonts/smalldevfont
Loading fonts/consoleFont
Loading fonts/bigfont
Loading fonts/smallfont
Loading fonts/consolefont
Loading fonts/boldfont
Loading fonts/normalfont
Loading fonts/extrabigfont
Loading fonts/objectivefont
Loading fonts/hudbigfont
Loading fonts/hudsmallfont
*/

int __cdecl LoadCustomFont(const char *Name)
{
	//Log::Debug("CustomFont", Name);
	Com_Printf(va("Loading %s", Name));

	if(strstr(Name, "fonts/consoleFont"))
	{
		return (int)DB_FindXAssetHeader(0x18, "fonts/consoleFont", 1);
	}
	if (strstr(Name, "fonts/normalfont"))
	{
		void* font;
		font = DB_FindXAssetHeader(0x18, "fonts/normalfont", 1);

		//Draw(font);
		return (int)font;
	}

	return (int)DB_FindXAssetHeader(0x18, Name, 1);
}

void WriteToFile(const char *Name, int File, int size)
{
	Log::Debug("", Name);
	char filename[6000];

	char dir[512];
	sprintf(filename, "PlusIW5/%s", (const char*)Name);

	FILE* file = fopen(filename, "w");

	if (file)
	{
		fprintf(file, "%s", File);
		fflush(file);
		fclose(file);
	}
}

int64_t GetFileSize(const char *Filename)
{
	std::ifstream File(Filename, std::ios::binary);
	std::streamsize Size = 0;

	// ifstream::tellg() returns -1 if there's no file.
	File.seekg(0, std::ios::end);
	Size = File.tellg();
	File.seekg(0, std::ios::beg);

	File.close();
	return Size;
}

bool FileExists(const char *Filename)
{
	return GetFileSize(Filename) != -1;
}

bool ReadFile(const char* Filename, void* DataBuffer)
{
	// Waste of time.
	if (!FileExists(Filename))
		return false;
	std::ifstream File(Filename, std::ios::binary);
	if (!File.read((char*)DataBuffer, GetFileSize(Filename)))
	{
		File.close();
		return false;
	}
	else
	{
		File.close();
		return true;
	}
}

int RawReader(const char *Name)
{
	//Com_Printf(va("Reading %s", Name));

	/*if (!FileExists(Name))
	Log::Debug("RawFile Read", "%s not exist", Name);
	return 0;*/

	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen(Name, "rb");
	if (pFile == NULL) { fputs("File error", stderr); exit(1); }

	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL) { Log::Debug("RawFile Read", "%s memory error", Name); }

	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) { Log::Debug("RawFile Read", "%s reading error", Name); }

	//Log::Debug("RawFile Read", "%s Readed size: %d", Name, result);

	fclose(pFile);

	return (int)buffer;
	/*
	if (!File.read((char*)newRaw, GetFileSize(Name)))
	{
	File.close();
	Log::Debug("RawFile Read", "%s not exist", newRaw);
	return newRaw;
	}
	else
	{
	File.close();
	return newRaw;
	}*/
}

int LoadCustomRawFiles(const char *Name, char *File, int a3)
{

	//Com_Printf(va("Loading RawFiles %s", Name));

	char filename[6000];

	char dir[512];
	sprintf(filename, "PlusIW5/%s", (const char*)Name);

	Com_Printf(va("Loading %s", filename));

	if(!Name)
		return (int)File;

	int v3;
	int result;

	v3 = (int)DB_FindXAssetHeader(0x26, Name, 1);

	if (exist(0x26, (int)Name))
	{
		result = 0;
	}
	else
	{
		Read(v3, File, a3);
		if (strstr(Name, "mp/playeranim.script") || strstr(Name, "mp/playeranimtypes.txt"))
		{
			result = (int)File;
			return result;
		}
		if (FileExists(filename))
		{
			return (int)RawReader(filename);
		}
		else
		{
			result = (int)File;
			WriteToFile(Name, result, a3);
		}
	}

	return result;
}

void Patch_IW5Materials()
{
	if (*(DWORD*)0x401B40 == 0x4304720C)//mp
	{
		CustomFont.Initialize(0x485440, LoadCustomFont);
		CustomFont.InstallHook();

		//CustomMaterial.Initialize(0x4CE2F0, LoadCustomRawFiles);
		//CustomMaterial.InstallHook();

		//CustomMaterial2.Initialize(0x4A5CA0, LoadCustomRawFiles);
		//CustomMaterial2.InstallHook();

		//CustomMaterial2.Initialize(0x514930, LoadCustomRawFiles);
		//CustomMaterial2.InstallHook();

		//CustomMaterial3.Initialize(0x53FA00, LoadCustomRawFiles);
		//CustomMaterial3.InstallHook();
		
		//CustomMaterial3.Initialize(0x6349B0, LoadCustomRawFiles);
		//CustomMaterial3.InstallHook();
	}
	else if (*(DWORD*)0x401B40 == 0x22E8A300)//sp
	{
		CustomFont.Initialize(0x45D040, LoadCustomFont);
		CustomFont.InstallHook();
	}
}