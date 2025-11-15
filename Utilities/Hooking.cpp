#include "..\stdafx.h"

#pragma unmanaged
void Hook::Quick::NOP(void *Address, uint32_t Length)
{
	memset(Address, 0x90, Length);
}
void Hook::Quick::Call(void *Address, uint32_t Data, CallPatch PatchType)
{
	switch (PatchType)
	{
	case CallPatch::PATCH_JUMP:
		*(uint8_t*)Address = (uint8_t)0xE9;
		break;

	case CallPatch::PATCH_CALL:
		*(uint8_t*)Address = (uint8_t)0xE8;
		break;
	default:
		break;
	}

	*(uint32_t *)((uint32_t)Address + 1) = (uint32_t)Data - (uint32_t)Address - 5;
}
void Hook::Stomp::Initialize(uintptr_t Place, void *HookToInstall, uint8_t ByteCount, bool useJump)
{
	_Place = (uint8_t *)Place;
	_ByteCount = ByteCount < sizeof(_OriginalCode) ? ByteCount : sizeof(_OriginalCode);
	memcpy(_OriginalCode, _Place, sizeof(_OriginalCode));
	_Hook = HookToInstall;
	_Jump = useJump;
}
void Hook::Stomp::InstallHook(void *HookToInstall)
{
	if (HookToInstall)
		_Hook = HookToInstall;

	if (_Hook)
	{
		DWORD d = 0;
		VirtualProtect(_Place, _ByteCount, PAGE_READWRITE, &d);
		memset(_Place, 0x90, _ByteCount);
		_Place[0] = _Jump ? (uint8_t)0xE9 : (uint8_t)0xE8;
		*(DWORD*)(_Place + 1) = (uint8_t *)_Hook - _Place - 5;
		VirtualProtect(_Place, _ByteCount, d, &d);
	}
}
void Hook::Stomp::ReleaseHook()
{
	DWORD d = 0;
	VirtualProtect(_Place, _ByteCount, PAGE_READWRITE, &d);
	memcpy(_Place, _OriginalCode, _ByteCount);
	VirtualProtect(_Place, _ByteCount, d, &d);
}
void Hook::Stomp::PermanentHook(uintptr_t Place, void *HookToInstall)
{
	memset((uint8_t *)Place, 0x90, 5);
	*(uint8_t *)Place = 0xE9;
	*(DWORD*)((uint8_t *)Place + 1) = (uint8_t *)HookToInstall - (uint8_t *)Place - 5;
}
void Hook::Call::Initialize(uint32_t Place, void *HookToInstall)
{
	_Place = (uint8_t *)Place;
	memcpy(_OriginalCode, _Place, sizeof(_OriginalCode));

	_Original = _Place + sizeof(_OriginalCode) + *(ptrdiff_t *)(_OriginalCode + 1);
	_Hook = HookToInstall;
}
void Hook::Call::InstallHook(void *HookToInstall)
{
	if (HookToInstall)
		_Hook = HookToInstall;

	if (_Hook)
		*(ptrdiff_t *)(_Place + 1) = (uint8_t *)_Hook - _Place - 5;
}
void Hook::Call::ReleaseHook()
{
	memcpy(_Place + 1, _OriginalCode + 1, sizeof(_OriginalCode) - 1);
}
void Hook::Call::PermanentHook(uint32_t Place, void *HookToInstall)
{
	*(ptrdiff_t *)((uint8_t *)Place + 1) = (uint8_t *)HookToInstall - (uint8_t *)Place - 5;
}



/*Hooking::Hook::Hook(bool uninstallOnDestruct)
{
	this->uninstallOnDestruct = uninstallOnDestruct;
}

Hooking::Hook::Hook(const Hook& hook)
{
	this->hookAddress = hook.hookAddress;
	this->uninstallOnDestruct = hook.uninstallOnDestruct;
	this->originalData = hook.originalData;
}

Hooking::Hook::Hook(const std::uintptr_t& address, bool uninstallOnDestruct)
{
	this->hookAddress = address;
	this->uninstallOnDestruct = uninstallOnDestruct;
}

Hooking::Hook::~Hook()
{
	if (this->uninstallOnDestruct)
	{
		this->Uninstall();
	}
}

Hooking::Hook* Hooking::Hook::Install(const std::uintptr_t& address)
{
	this->hookAddress = address;
	return this;
}

void Hooking::Hook::Uninstall()
{
	if (!this->originalData.empty())
	{
		auto protection = Hook::Unprotect(this->hookAddress, this->originalData.size());
		memcpy(reinterpret_cast<void*>(this->hookAddress), &this->originalData[0], this->originalData.size());
		Hook::Protect(this->hookAddress, this->originalData.size(), protection);
	}
}

Hooking::Hook::Ptr Hooking::Hook::make_shared(bool uninstallOnDestruct)
{
	return std::make_shared < Hooking::Hook >(uninstallOnDestruct);
}

Hooking::Hook::Ptr Hooking::Hook::make_shared(const Hook& hook)
{
	return std::make_shared < Hooking::Hook >(hook);
}

Hooking::Hook::Ptr Hooking::Hook::make_shared(const std::uintptr_t& address, bool uninstallOnDestruct)
{
	return std::make_shared < Hooking::Hook >(address, uninstallOnDestruct);
}

void Hooking::Hook::Nop(const std::size_t& size)
{
	this->SetOriginalData(size);
	Hooking::Hook::Nop(this->hookAddress, size);
}

void Hooking::Hook::Nop(const std::uintptr_t& address, const std::size_t& size)
{
	auto protection = Hook::Unprotect(address, size);
	memset(reinterpret_cast<void*>(address), 0x90, size);
	Hook::Protect(address, size, protection);
}

void Hooking::Hook::Write(const std::vector < std::uint8_t >& bytes)
{
	this->SetOriginalData(bytes.size());
	Hooking::Hook::Write(this->hookAddress, bytes);
}

void Hooking::Hook::Write(const std::uintptr_t& address, const std::vector < std::uint8_t >& bytes)
{
	auto protection = Hook::Unprotect(address, bytes.size());
	memcpy(reinterpret_cast<void*>(address), &bytes[0], bytes.size());
	Hook::Protect(address, bytes.size(), protection);
}

void Hooking::Hook::WriteString(const std::string& string)
{
	this->SetOriginalData(string.size() + 1);
	Hooking::Hook::WriteString(this->hookAddress, string);
}

void Hooking::Hook::WriteString(const std::uintptr_t& address, const std::string& string)
{
	auto protection = Hook::Unprotect(address, string.size());
	memcpy(reinterpret_cast<void*>(address), &string[0], string.size());
	Hook::Protect(address, string.size(), protection);
}

std::uint32_t Hooking::Hook::Unprotect(const std::uintptr_t& address, const std::size_t& size)
{
	return Memprotect::Unprotectrange(reinterpret_cast<void*>(address), size);
}

void Hooking::Hook::Protect(const std::uintptr_t& address, const std::size_t& size, const std::uint32_t& protection)
{
	return Memprotect::Protectrange(reinterpret_cast<void*>(address), size, protection);
}*/


PIMAGE_NT_HEADERS xImageNTHeader(uint64_t ImgBase)
{
	PIMAGE_DOS_HEADER DosHeader;
	PIMAGE_NT_HEADERS Image;

	DosHeader = (PIMAGE_DOS_HEADER)ImgBase;

	if (DosHeader->e_magic == IMAGE_DOS_SIGNATURE)
	{
		Image = reinterpret_cast<PIMAGE_NT_HEADERS> (DosHeader->e_lfanew + ImgBase);
		if (Image->Signature == IMAGE_NT_SIGNATURE)
		{
			return (PIMAGE_NT_HEADERS)(DosHeader->e_lfanew + ImgBase);
		}
	}
	return 0;
}
size_t GetIATAddress(const char* ModuleName, const char* FunctionName, uint64_t ImgBase)
{
	PIMAGE_NT_HEADERS Image;
	PIMAGE_IMPORT_DESCRIPTOR ImportDesc;
	PIMAGE_THUNK_DATA ThunkData;

	Image = xImageNTHeader(ImgBase);
	if (!Image)
		return 0;

	ImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((Image->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress) + (size_t)ImgBase);
	for (int i = 0;; i++)
	{
		if (ImportDesc->Name == NULL)
			break;

		if (!_stricmp((char *)(ImportDesc->Name + ImgBase), ModuleName))
		{
			if ((ImportDesc->OriginalFirstThunk) != NULL)
			{
				for (int c = 0;; c++)
				{
					ThunkData = (PIMAGE_THUNK_DATA)((size_t)ImportDesc->OriginalFirstThunk + c * sizeof(IMAGE_THUNK_DATA) + ImgBase);

					if (ThunkData->u1.AddressOfData == NULL)
						break;

					if (!_stricmp((char *)(((PIMAGE_IMPORT_BY_NAME)((size_t)ThunkData->u1.AddressOfData + ImgBase))->Name), FunctionName))
					{
						return (size_t)(((PIMAGE_THUNK_DATA)((size_t)ImportDesc->FirstThunk + c * sizeof(IMAGE_THUNK_DATA) + (size_t)ImgBase))->u1.Function);
					}
				}
			}
		}
	}

	return 0;
}
size_t WriteIATAddress(const char* ModuleName, const char* FunctionName, uint64_t ImgBase, void *NewProc)
{
	PIMAGE_NT_HEADERS Image;
	PIMAGE_IMPORT_DESCRIPTOR ImportDesc;
	PIMAGE_THUNK_DATA ThunkData;
	size_t oldValue;

	Image = xImageNTHeader(ImgBase);

	if (!Image) return 0;

	ImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((Image->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress) + (size_t)ImgBase);

	HMODULE origHandle = nullptr;
	LPVOID origProc = NULL;

	if (ModuleName != NULL)
		origHandle = GetModuleHandle(ModuleName);
	if (origHandle != nullptr)
		origProc = GetProcAddress(origHandle, FunctionName);
	else
		return 0;

	for (int i = 0;; i++)
	{
		if (ImportDesc->Name == NULL) break;

		if (!_stricmp((char*)((size_t)ImportDesc->Name + (size_t)ImgBase), ModuleName))
		{
			if ((ImportDesc->OriginalFirstThunk) != 0)
			{
				for (int c = 0;; c++)
				{
					ThunkData = (PIMAGE_THUNK_DATA)((size_t)ImportDesc->OriginalFirstThunk + c * sizeof(IMAGE_THUNK_DATA) + (size_t)ImgBase);

					if (ThunkData->u1.AddressOfData == NULL) break;

					if (ThunkData->u1.AddressOfData & 0x80000000)
					{
						uint32_t ordinalNumber = ThunkData->u1.AddressOfData & 0xFFFFFFF;

						if (GetProcAddress(origHandle, (LPCSTR)ordinalNumber) == origProc)
						{
							oldValue = (uint32_t)(((PIMAGE_THUNK_DATA)((size_t)ImportDesc->FirstThunk + c * sizeof(IMAGE_THUNK_DATA) + (size_t)ImgBase))->u1.Function);
							((PIMAGE_THUNK_DATA)((size_t)ImportDesc->FirstThunk + c * sizeof(IMAGE_THUNK_DATA) + (size_t)ImgBase))->u1.Function = (size_t)NewProc;
							return oldValue;
						}
					}
					else
					{
						if (!_stricmp((char*)(((PIMAGE_IMPORT_BY_NAME)((size_t)ThunkData->u1.AddressOfData + (size_t)ImgBase))->Name), FunctionName))
						{
							oldValue = (size_t)(((PIMAGE_THUNK_DATA)((size_t)ImportDesc->FirstThunk + c * sizeof(IMAGE_THUNK_DATA) + (size_t)ImgBase))->u1.Function);
							((PIMAGE_THUNK_DATA)((size_t)ImportDesc->FirstThunk + c * sizeof(IMAGE_THUNK_DATA) + (size_t)ImgBase))->u1.Function = (size_t)NewProc;
							return oldValue;
						}
					}
				}
			}
		}
		ImportDesc++;
	}

	return 0;
}
size_t RedirectIAT(const char *Modulename, const char *Functionname, void *NewFunction)
{
	size_t Imagebase;
	size_t OriginalEntry;
	PIMAGE_DOS_HEADER DOSHeader;
	PIMAGE_NT_HEADERS NTHeader;
	PIMAGE_IMPORT_DESCRIPTOR Imports;
	PIMAGE_THUNK_DATA ImportThunkData;

	// Initialize host information.
	Imagebase = (size_t)GetModuleHandleA(NULL);
	OriginalEntry = (size_t)GetProcAddress(GetModuleHandleA(Modulename), Functionname);

	// Fetch information from the PE header.
	DOSHeader = (PIMAGE_DOS_HEADER)Imagebase;
	NTHeader = (PIMAGE_NT_HEADERS)(DOSHeader->e_lfanew + Imagebase);

	// Verify that the import table has not been destroyed by a packer.
	if (NTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0)
	{
		//Log::Error("RedirectIAT", hString::va("%s: The current application does not have an import table.", __FUNCTIONW__));
		return 0;
	}
	// Iterate through the import table until we find our enrty.
	Imports = (PIMAGE_IMPORT_DESCRIPTOR)((NTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress) + Imagebase);
	for (size_t i = 0; Imports[i].Characteristics != 0; ++i)
	{
		// Skip unrelated modules.
		if (_stricmp((char *)(Imports[i].Name + Imagebase), Modulename))
			continue;

		// Iterate through the thunks.
		for (size_t c = 0;; ++c)
		{
			// Fetch the next thunk and verify that it's not the last.
			ImportThunkData = (PIMAGE_THUNK_DATA)((size_t)Imports[i].OriginalFirstThunk + (c * sizeof(IMAGE_THUNK_DATA)) + Imagebase);
			if (ImportThunkData->u1.AddressOfData == NULL)
				break;

			// Verify the import by ordinal.
			if (ImportThunkData->u1.Ordinal & IMAGE_ORDINAL_FLAG)
			{
				PIMAGE_THUNK_DATA OrdinalThunk = (PIMAGE_THUNK_DATA)((size_t)Imports[i].FirstThunk + (c * sizeof(IMAGE_THUNK_DATA)) + Imagebase);

				if (OriginalEntry == OrdinalThunk->u1.Function)
				{
					OriginalEntry = OrdinalThunk->u1.Function;
					OrdinalThunk->u1.Function = (size_t)NewFunction;
					return OriginalEntry;
				}
				else
					continue;
			}

			// Verify the import by name.
			if (!_stricmp(((PIMAGE_IMPORT_BY_NAME)(ImportThunkData->u1.AddressOfData + Imagebase))->Name, Functionname))
			{
				PIMAGE_THUNK_DATA NameThunk = (PIMAGE_THUNK_DATA)((size_t)Imports[i].FirstThunk + (c * sizeof(IMAGE_THUNK_DATA)) + Imagebase);
				OriginalEntry = NameThunk->u1.Function;
				NameThunk->u1.Function = (size_t)NewFunction;
				return OriginalEntry;
			}
		}
	}
	return 0;
}
