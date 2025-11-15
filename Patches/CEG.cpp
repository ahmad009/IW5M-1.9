#include "..\stdafx.h"
#define MAX_SIGNATURES 256
#define cegRedirectFuncs(a, v) *(BYTE*)a = 0xB8; *(DWORD*)(a + 1) = v;

typedef struct signature_s
{
	unsigned char* signature;
	char* mask;
	size_t size;
	unsigned char* replace;
	int replaceOffset;
	size_t replaceSize;
	void(*replaceCB)(char* address, void* userData);
	void* userData;
	int logOffset;
} signature_t;

void ProcessSignature(int id, signature_t* signature)
{
	unsigned char *pBasePtr = (unsigned char *)0x400000;
	unsigned char *pEndPtr = pBasePtr + 0x800000;//GetImageLength() - 0xF000;
	size_t i;
	int occurred = 0;

	while (pBasePtr < pEndPtr) {
		for (i = 0; i < signature->size; i++) {
			if ((signature->mask[i] != '?') && (signature->signature[i] != pBasePtr[i]))
				break;
		}

		if (i == signature->size)
		{
			DWORD addy = ((DWORD)pBasePtr) + signature->logOffset;
			//printf(" | grep -v %x", addy);

			occurred++;
			

			// patch output
			if (signature->replaceCB)
			{
				signature->replaceCB((char*)(pBasePtr + signature->replaceOffset), signature->userData);
			}

			if (signature->replace)
			{
				memcpy(pBasePtr + signature->replaceOffset, signature->replace, signature->replaceSize);
				Log::Info("Anticode", va("0x%x / %d", addy, id));
				Log::Info("Anticode", va("0x%x / %d  + %i  =>", addy, id, signature->replaceOffset, signature->replace));

				Log::Info("Anticode", "%02X", signature->replace);
			}
		}

		pBasePtr++;
	}

	//Log::Info("Anticode", va("signature %d occurred %d times\n", id, occurred).c_str());
}

void PatchCEGCRC1(char* addr, void*)
{
	*(DWORD*)(addr) = 0x90909090;
	*(BYTE*)(addr + 4) = 0x90;
	*(DWORD*)(addr + 8) = 0xF883C033;
	*(WORD*)(addr + 12) = 0x9000;
}

void PatchCEGCRC2(char* addr, void*)
{
	*(DWORD*)(addr + 1) = 0x90909090;
	*(BYTE*)(addr + 5) = 0x90;
	*(DWORD*)(addr + 9) = 0xF883C033;
	*(WORD*)(addr + 13) = 0x9000;
}

void AntiCode_Assign(char* addr, void* variable)
{
	*(DWORD*)variable = (DWORD)addr;
}

void PatchSignatures()
{
	static signature_t signatures[19];
	memset(signatures, 0, sizeof(signatures));

	// type1 crc
	signatures[0].signature = (unsigned char*)"\xE8????\x83\xC4\x1C\x3B\x05????\x0F\x94\xC3\x84\xDB\x75";
	signatures[0].mask = "x????xxxxx????xxxxxx";
	signatures[0].size = 20;
	signatures[0].logOffset = 0;
	signatures[0].replaceCB = PatchCEGCRC1;
	signatures[0].replaceOffset = 0;

	// type2 crc
	signatures[1].signature = (unsigned char*)"\x57\xE8????\x83\xC4\x1C\x3B\x86????\x0F\x94\xC1\x83\xC6\x04";
	signatures[1].mask = "xx????xxxxx????xxxxxx";
	signatures[1].size = 20;
	signatures[1].logOffset = 0;
	signatures[1].replaceCB = PatchCEGCRC2;
	signatures[1].replaceOffset = 0;
	signatures[1].replaceSize = 7;

	//DISABLE_GUID_CHECK
	signatures[2].signature = (unsigned char*)"\x81\xEC??\x00\x00\xE8????\x84\xC0\x75\x06\xB0\x01";
	signatures[2].mask = "xx??xxx?????xxxxxx";
	signatures[2].size = 17;
	signatures[2].logOffset = 0;
	signatures[2].replace = (unsigned char*)"\x90\x90";
	signatures[2].replaceOffset = 13;
	signatures[2].replaceSize = 2;



	signatures[4].signature = (unsigned char*)"\x56\xE8????\x8B\xF0\xE8????\x50\x56\xE8";
	signatures[4].mask = "xx????xxx????xxx";
	signatures[4].size = 16;
	signatures[4].logOffset = 0;
	signatures[4].replace = (unsigned char*)"\xC3";
	signatures[4].replaceOffset = 0;
	signatures[4].replaceSize = 1;

	signatures[5].signature = (unsigned char*)"\x55\x8B\xEC\x83\xEC\x18\x53\x56\x57\xE8????\xE8????\x8B\xF0";
	signatures[5].mask = "xxxxxxxxxx????x????xx";
	signatures[5].size = 21;
	signatures[5].logOffset = 0;
	signatures[5].replace = (unsigned char*)"\xC3";
	signatures[5].replaceOffset = 0;
	signatures[5].replaceSize = 1;

	signatures[6].signature = (unsigned char*)"\x8B\x55\x08\x3B\x17\x0F\x82?\xFF\xFF\xFF\x5F\x5E\x32\xC0";
	signatures[6].mask = "xxxxxxx?xxxxxxx";
	signatures[6].size = 15;
	signatures[6].logOffset = 0;
	signatures[6].replace = (unsigned char*)"\x90\x90\x90\x90\x90\x90";
	signatures[6].replaceOffset = 5;
	signatures[6].replaceSize = 6;

	signatures[7].signature = (unsigned char*)"\x53\x55\x56\x6A\x00\x6A\x01\x68????\xFF\x15";
	signatures[7].mask = "xxxxxxxx????xx";
	signatures[7].size = 14;
	signatures[7].logOffset = 0;
	signatures[7].replace = (unsigned char*)"\x31\xC0\xC3\x90";
	signatures[7].replaceOffset = 0;
	signatures[7].replaceSize = 4;

	signatures[8].signature = (unsigned char*)"\x55\x8B\xEC\x83\xEC?\x53\x56\x57\x6A\x01\x8D\x45?\x50\xFF\x15";
	signatures[8].mask = "xxxxx?xxxxxxx?xxx";
	signatures[8].size = 17;
	signatures[8].logOffset = 0;
	signatures[8].replace = (unsigned char*)"\x33\xC0\x40\xC3";
	signatures[8].replaceOffset = 0;
	signatures[8].replaceSize = 4;

	signatures[9].signature = (unsigned char*)"\x84\xC0\x74?\x53\x53";
	signatures[9].mask = "xxx?xx";
	signatures[9].size = 6;
	signatures[9].logOffset = 0;
	signatures[9].replace = (unsigned char*)"\xEB";
	signatures[9].replaceOffset = 2;
	signatures[9].replaceSize = 1;

	signatures[10].signature = (unsigned char*)"\xE8????\x83\xC4\x1C\x83\xF8\x14\x0F\x94\xC3\x84\xDB\x75";
	signatures[10].mask = "x????xxxxxxxxxxxx";
	signatures[10].size = 17;
	signatures[10].logOffset = 0;
	signatures[10].replace = (unsigned char*)"\xB8\x14\x00\x00";
	signatures[10].replaceOffset = 0;
	signatures[10].replaceSize = 4;

	signatures[11].signature = (unsigned char*)"\xE8????\x83\xC4\x1C\x85\xC0\x0F\x94\xC3\x84\xDB\x75";
	signatures[11].mask = "x????xxxxxxxxxxx";
	signatures[11].size = 16;
	signatures[11].logOffset = 0;
	signatures[11].replace = (unsigned char*)"\xB8\x00\x00\x00";
	signatures[11].replaceOffset = 0;
	signatures[11].replaceSize = 4;

	signatures[12].signature = (unsigned char*)"\x84\xC0\x75\x06\xB0\x01\x8B\xE5";
	signatures[12].mask = "xxxxxxxx";
	signatures[12].size = 8;
	signatures[12].logOffset = 0;
	signatures[12].replace = (unsigned char*)"\x90\x90";
	signatures[12].replaceOffset = 2;
	signatures[12].replaceSize = 2;

	signatures[13].signature = (unsigned char*)"\x55\x8B\xEC\xB8????\xE8????\xE8????\x84\xC0\x75?\xB0\x01";
	signatures[13].mask = "xxxx????x????x????xxx?xx";
	signatures[13].size = 24;
	signatures[13].logOffset = 0;
	signatures[13].replace = (unsigned char*)"\x90\x90";
	signatures[13].replaceOffset = 20;
	signatures[13].replaceSize = 2;

	// DB_FindXAssetHeader
	signatures[14].signature = (unsigned char*)"\x51\x53\x8B\x1D????\x55\x56\x57\x8B\x7C\x24\x18\xC7";
	signatures[14].mask = "xxxx????xxxxxxxx";
	signatures[14].size = 16;
	signatures[14].logOffset = 0;
	signatures[14].replaceCB = AntiCode_Assign;
	signatures[14].replaceOffset = 0;
	signatures[14].userData = &DB_FindXAssetHeader;


	//DW ProcessFileRequest Disable
	/*signatures[15].signature = (unsigned char*)"\x83\xEC\x40\x56\x8B\x74\x24\x48\x83\x7E\x08\x00\x75\x21\x6A\x40\x8D\x44\x24\x08\x50\x6A\x04";
	signatures[15].mask = "xxxxxxxxxxxxxxxxxxxxxxx";
	signatures[15].size = 23;
	signatures[15].logOffset = 0;
	signatures[15].replace = (unsigned char*)"\x33\xC0\x40\xC3";
	signatures[15].replaceOffset = 0;
	signatures[15].replaceSize = 4;*/

	//DISABLE_STEAM_IPCS_PTR
	/*signatures[15].signature = (unsigned char*)"\x83\xEC\x48\x53\x55\x56\x57\x6a\x01\x8d\x44\x24\x34\x50\x33\xed\xff\x15";//
	signatures[15].mask = "xxxxxxxxxxxxxxxxxx";
	signatures[15].size = 18;
	signatures[15].logOffset = 0;
	signatures[15].replace = (unsigned char*)"\x33\xc0\x40\xc3";
	signatures[15].replaceOffset = 0;
	signatures[15].replaceSize = 4;*/


	for (int i = 0; i < 18; i++)
	{
		signature_t* signature = &signatures[i];

		if (signature->signature)
		{
			ProcessSignature(i, signature);
		}
	}
}

void Anticode()
{
	PatchSignatures();

	if (*(DWORD*)0x401B40 == 0x4304720C)
	{
		cegRedirectFuncs(0x462AF0, 0x4CC280);//
		cegRedirectFuncs(0x536BE0, 0x52E330);//
		cegRedirectFuncs(0x567870, 0x643C00);//
		cegRedirectFuncs(0x570710, 0x43E680);//
		cegRedirectFuncs(0x57C1C0, 0x4BCE90);//
		cegRedirectFuncs(0x4B4700, 0x641030);
		cegRedirectFuncs(0x4B4700, 0x641030);
		cegRedirectFuncs(0x424D20, 0x529160);
	}
	else if (*(DWORD*)0x401B40 == 0x22E8A300)
	{
		cegRedirectFuncs(0x4D0020, 0x4ACB40);
		cegRedirectFuncs(0x4CA310, 0x48A8E0);
	}
}