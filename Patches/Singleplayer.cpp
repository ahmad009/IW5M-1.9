#include "..\stdafx.h"

void Patch_IW5Console();
void Patch_IW5FPS();
void Patch_IW5Dvars();

typedef LRESULT(__cdecl* sub_4C84E0_t)(const char *a1);
sub_4C84E0_t sub_4C84E0 = (sub_4C84E0_t)0x4C84E0;

static char ConsolePrint_vaBuffer[4][4096];
static int ConsolePrint_vaNextBufferIndex = 0;

void ConsolePrint(const char *fmt, ...)
{
	va_list ap;
	char *dest = &ConsolePrint_vaBuffer[ConsolePrint_vaNextBufferIndex][0];
	ConsolePrint_vaNextBufferIndex = (ConsolePrint_vaNextBufferIndex + 1) % 4;
	va_start(ap, fmt);
	vsprintf(dest, fmt, ap);
	va_end(ap);
	sub_4C84E0(dest);
	sub_4C84E0("\n");
}

DWORD stackspot1;
DWORD stackspot2;
DWORD stackspot3;

// Stackfix1
void __declspec(naked) xx1()
{
	__asm
	{
		MOV EAX, ESP
		SUB ESP, 0x38
		restart:
		CMP ESP, EAX
			JE exit1
			MOV DWORD PTR DS : [ESP], 0
			ADD ESP, 4
			JMP restart
			exit1 :
		SUB ESP, 0x38
			PUSH 0x14001
			JMP stackspot1
	}
}

// Stackfix2
void __declspec(naked) xx2()
{
	__asm
	{
		MOV EAX, ESP
		SUB ESP, 0x40
		restart:
		CMP ESP, EAX
			JE exit1
			MOV DWORD PTR DS : [ESP], 0
			ADD ESP, 4
			JMP restart
			exit1 :
		SUB ESP, 0x40
			PUSH 0x4000
			JMP stackspot2
	}
}

// Stackfix3
void __declspec(naked) xx3()
{
	__asm
	{
		MOV EAX, ESP
		SUB ESP, 0x91C
		restart:
		CMP ESP, EAX
			JE exit1
			MOV DWORD PTR DS : [ESP], 0
			ADD ESP, 4
			JMP restart
			exit1 :
		MOV EAX, DWORD PTR DS : [ESP + 8]
			SUB ESP, 0x91C
			JMP stackspot3
	}
}

void Patch_IW5SP()
{
	Patch_IW5FPS();
	Patch_IW5Console();
	Patch_IW5Dvars();

	*(BYTE *)0x53CBDC = 0xEB;

	QCALL(0x60DABA, ConsolePrint, QPATCH_CALL);
	QCALL(0x60DAD1, ConsolePrint, QPATCH_CALL);
	//*(BYTE *)0x607AA8 = 0;
	//*(BYTE *)0x607AAA = 1;
	QNOP(0x608AD1, 5);
	//QNOP(0x585537, 5);
	
	((LRESULT(__cdecl*)())0x470AF0)();


	// Stats stuff
	*(BYTE*)0x663C54 = 0xEB;
	//*(BYTE*)0x663B5A = 0xEB;

	//netfix
	/**(WORD *)0x507F40 = 0x9090;
	*(WORD *)0x517DDC = 0x9090;
	*(WORD *)0x5FCCBD = 0x9090;
	DWORD netfix4 = 0x62C98A;//0x62C7B5;
	*(DWORD *)netfix4 = 0x90909090;
	netfix4 += 4;
	*(WORD *)netfix4 = 0x9090;

	//NETFIX5
	*(BYTE *)0x503F8D = 0xEB;

	//lobbyfix
	*(BYTE *)0x546454 = 0xEB;
	*(WORD *)0x5592F3 = 0x9090;
	*(WORD *)0x513491 = 0x9090;
	*(WORD*)0x5134C6 = 0x9090;

	//netport
	*(WORD*)0x431928 = 0x0C06; //0x7200
	*(WORD*)0x62f87f = 0x0C06;//0x62F87A
	*(WORD*)0x62fa54 = 0x0C06;//0x62FA4F

	//joinfix
	*(BYTE *)0x5503EE = 0xEB;

	//xlivefix
	*(WORD*)0x57F271 = 0x9090;*/


	DWORD netfix1 = Findpattern(Pattern::Textsegment, "750B8B3D????E9");
	DWORD netfix2 = Findpattern(Pattern::Textsegment, "3B3D????75116A1EE8") + 6;
	DWORD netfix3 = Findpattern(Pattern::Textsegment, "83FF0175?8B8680000000") + 3;
	DWORD netfix4 = Findpattern(Pattern::Textsegment, "837C2410000F84?00000033DB") + 5;
	DWORD lobbyfix1 = Findpattern(Pattern::Textsegment, "84C0751A8B4C2410") + 2;
	DWORD lobbyfix2 = Findpattern(Pattern::Textsegment, "8BCEE8????84C07416FF15") + 9;
	DWORD lobbyfix3 = Findpattern(Pattern::Textsegment, "8BF139107561") + 4;
	DWORD lobbyfix4 = Findpattern(Pattern::Textsegment, "381D????756E") + 6;

	DWORD netport1 = Findpattern(Pattern::Textsegment, "6A006888690000") + 3;
	*(WORD*)netport1 = 0x7200;

	DWORD netport2 = Findpattern(Pattern::Textsegment, "6A006888690000") + 3;
	*(WORD*)netport2 = 0x7200;

	DWORD netport3 = Findpattern(Pattern::Textsegment, "6A006888690000") + 3;
	*(WORD*)netport3 = 0x7200;

	DWORD joinfix = Findpattern(Pattern::Textsegment, "83C40884C0751A8B4C2404") + 5;
	DWORD xlivefix = Findpattern(Pattern::Textsegment, "83C41485C0741CA1") + 5;
	DWORD stackfix1 = Findpattern(Pattern::Textsegment, "83EC386801400100");
	DWORD stackfix2 = Findpattern(Pattern::Textsegment, "83EC406800400000");
	DWORD stackfix3 = Findpattern(Pattern::Textsegment, "8B44240881EC1C090000");
	DWORD netfix5 = Findpattern(Pattern::Textsegment, "7FED8B44240C");
	DWORD fovvar1 = Findpattern(Pattern::Textsegment, "741CA1????6A00") + 3;
	DWORD fovvar2 = Findpattern(Pattern::Textsegment, "74298B15????6A00") + 4;

	*(WORD *)netfix1 = 0x9090;

	*(WORD *)netfix2 = 0x9090;

	*(WORD *)netfix3 = 0x9090;

	*(DWORD *)netfix4 = 0x90909090;
	netfix4 += 4;
	*(WORD *)netfix4 = 0x9090;

	*(BYTE *)netfix5 = 0xEB;

	*(BYTE *)lobbyfix1 = 0xEB;

	*(WORD *)lobbyfix2 = 0x9090;

	*(WORD *)lobbyfix3 = 0x9090;

	*(WORD*)lobbyfix4 = 0x9090;

	*(WORD*)netport1 = 0x7200;

	*(WORD*)netport2 = 0x7200;

	*(WORD*)netport3 = 0x7200;

	*(BYTE *)joinfix = 0xEB;

	*(WORD*)xlivefix = 0x9090;

	// Stackfix1
	// E9 NOP NOP NOP to hook

	stackspot1 = stackfix1 + 8;
	DWORD stackfunc1 = (DWORD)xx1;
	DWORD jumpdw1 = (stackfunc1 - stackfix1) - 5;
	*(BYTE *)stackfix1 = 0xE9;
	stackfix1++;
	*(DWORD *)stackfix1 = jumpdw1;


	// Stackfix2
	// E9 NOP NOP NOP to hook

	stackspot2 = stackfix2 + 8;
	DWORD stackfunc2 = (DWORD)xx2;
	DWORD jumpdw2 = (stackfunc2 - stackfix2) - 5;
	*(BYTE *)stackfix2 = 0xE9;
	stackfix2++;
	*(DWORD *)stackfix2 = jumpdw2;

	// Stackfix3
	// E9 NOP NOP NOP NOP NOP

	stackspot3 = stackfix3 + 10;
	DWORD stackfunc3 = (DWORD)xx3;
	DWORD jumpdw3 = (stackfunc3 - stackfix3) - 5;
	*(BYTE *)stackfix3 = 0xE9;
	stackfix3++;
	*(DWORD *)stackfix3 = jumpdw3;

	





	// Allow offline coop
	QNOP(0x4B8C07, 2);
	*(BYTE*)0x5503EE = 0xEB; // Steam connect
	*(BYTE*)0x546454 = 0xEB;
}