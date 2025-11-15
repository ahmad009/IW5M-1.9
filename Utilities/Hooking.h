#pragma once

/*namespace Hooking
{
	class Hook
	{
	public:
		using Ptr = typename std::shared_ptr < Hook >;

		Hook(bool uninstallOnDestruct = true);
		Hook(const Hook& hook);
		Hook(const std::uintptr_t& address, bool uninstallOnDestruct = true);
		~Hook();

		Hook* Install(const std::uintptr_t& address);
		void Uninstall();

		static Ptr make_shared(bool uninstallOnDestruct = true);
		static Ptr make_shared(const Hook& hook);
		static Ptr make_shared(const std::uintptr_t& address, bool uninstallOnDestruct = true);

		void Nop(const std::size_t& size);
		static void Nop(const std::uintptr_t& address, const std::size_t& size);
		void Write(const std::vector < std::uint8_t >& bytes);
		static void Write(const std::uintptr_t& address, const std::vector < std::uint8_t >& bytes);
		void WriteString(const std::string& string);
		static void WriteString(const std::uintptr_t& address, const std::string& string);

		template <typename T> void Set(const T& value)
		{
			this->SetOriginalData(sizeof T);
			Hook::Set<T>(this->hookAddress, value);
		}
		template <typename T> static void Set(const std::uintptr_t& address, const T& value)
		{
			auto protection = Hook::Unprotect(address, sizeof T);
			memcpy(reinterpret_cast<void*>(address), &value, sizeof T);
			Hook::Protect(address, sizeof T, protection);
		}

		template <typename T> void Call(const T& dest)
		{
#ifdef ENVIRONMENT64
			this->SetOriginalData(12);
#else
			this->SetOriginalData(5);
#endif

			Hook::Call<T>(this->hookAddress, dest);
		}

		template <typename T> static void Call(const std::uintptr_t& address, const T& dest)
		{
#ifdef ENVIRONMENT64
			auto protection = Hook::Unprotect(address, 12);
			Hook::Set<std::uint8_t>(address, 0x48);
			Hook::Set<std::uint8_t>(address + 1, 0xB8);
			Hook::Set<std::uintptr_t>(address + 2, std::uintptr_t(dest));
			Hook::Set<std::uint8_t>(address + 10, 0xFF);
			Hook::Set<std::uint8_t>(address + 11, 0xD0);
			Hook::Protect(address, 12, protection);
#else
			auto protection = Hook::Unprotect(address, 5);
			Hook::Set<std::uint8_t>(address, 0xE8);
			Hook::Set<std::uint32_t>(address + 1, std::uintptr_t(dest) - address - 5);
			Hook::Protect(address, 5, protection);
#endif
		}

		template <typename T> void Jump(const T& dest)
		{
#ifdef ENVIRONMENT64
			this->SetOriginalData(12);
#else
			this->SetOriginalData(5);
#endif

			Hook::Jump<T>(this->hookAddress, dest);
		}

		template <typename T> static void Jump(const std::uintptr_t& address, const T& dest)
		{
#ifdef ENVIRONMENT64
			auto protection = Hook::Unprotect(address, 12);
			Hook::Set<std::uint8_t>(address, 0x48);
			Hook::Set<std::uint8_t>(address + 1, 0xB8);
			Hook::Set<std::uintptr_t>(address + 2, std::uintptr_t(dest));
			Hook::Set<std::uint8_t>(address + 10, 0xFF);
			Hook::Set<std::uint8_t>(address + 11, 0xE0);
			Hook::Protect(address, 12, protection);
#else
			auto protection = Hook::Unprotect(address, 5);
			Hook::Set<std::uint8_t>(address, 0xE9);
			Hook::Set<std::uint32_t>(address + 1, std::uintptr_t(dest) - address - 5);
			Hook::Protect(address, 5, protection);
#endif
		}

		template <typename T> std::function<T> Detour(const T& dest, const std::size_t& size)
		{
			this->SetOriginalData(size);
			return Hook::Detour(this->hookAddress, dest, size);
		}

		template <typename T> static std::function<T> Detour(const std::uintptr_t& address, const T& dest, const std::size_t& size)
		{
#ifdef _WIN32
#ifdef ENVIRONMENT64
			static const constexpr auto jumpSize = 12;
#else
			static const constexpr auto jumpSize = 5;
#endif
			auto trampoline = VirtualAlloc(nullptr, size + jumpSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			memcpy(trampoline, reinterpret_cast<char*>(address), size + jumpSize);

			// redirect function to our function
			Hook::Jump<T>(address, dest);

			// redirect trampoline to the original function
			Hook::Jump(reinterpret_cast<std::uintptr_t>(trampoline) + size, address + size);

			// return trampoline
			return std::function<T>(reinterpret_cast<T*>(trampoline));
#else
			throw "Detour is not implemented for UNIX based systems.";
#endif
		}

	private:
		static std::uint32_t Unprotect(const std::uintptr_t& address, const std::size_t& size);
		static void Protect(const std::uintptr_t& address, const std::size_t& size, const std::uint32_t& protection);

		void SetOriginalData(const std::size_t& size)
		{
			if (this->originalData.empty())
			{
				this->originalData.resize(size);
				memcpy(&this->originalData[0], reinterpret_cast<void*>(this->hookAddress), size);
			}
		}

	protected:
		bool							uninstallOnDestruct;
		std::vector < std::uint8_t >	originalData;
		std::uintptr_t					hookAddress;

	};



	// Call the hooked function, maybe wrap in a a mutex.
#define Callhook_noret(Hook, ...) { Hook.Remove(); Hook.Function(__VA_ARGS__); Hook.Reinstall(); }
#define Callhook_ret(Hook, Result, ...) { Hook.Remove(); Result = Hook.Function(__VA_ARGS__); Hook.Reinstall(); }

	// Insert a jump at the address, optionally follows other jumps.
	template <typename Signature = void(void)>
	struct Stomphook
	{
		std::function<Signature> Function;
		uint8_t Savedtext[14]{};
		void *Savedlocation;
		void *Savedtarget;

		void Remove()
		{
			auto Protection = Memprotect::Unprotectrange(Savedlocation, 14);
			{
				std::memcpy(Savedlocation, Savedtext, 14);
			}
			Memprotect::Protectrange(Savedlocation, 14, Protection);
		}
		void Reinstall() { Install(Savedlocation, Savedtarget); }
		static Stomphook Install(void *Location, void *Target, bool Followjumps = true)
		{
			uint8_t *Address = (uint8_t *)Location;
			Stomphook<Signature> Localhook;

			// Check the first byte and increment the address.
			while (Followjumps)
			{
				switch (*(uint8_t *)Address)
				{
				case 0xE9:
				{
					Address += sizeof(uint8_t);
					Address += *(uint32_t *)Address;
					Address += sizeof(uint32_t);
					break;
				}
				case 0xFF:
				{
					if (*(uint16_t *)Address == 0x25FF)
					{
						Address += sizeof(uint16_t);
						Address += *(uint32_t *)Address;
						Address += sizeof(uint32_t);
						Address = (uint8_t *)(*(size_t *)(Address));
						break;
					}
					Followjumps = false;
					break;
				}

				/*
				TODO(Convery):
				Add more cases.
				*/

				/*default: Followjumps = false;
				}
			}

			// Save the current information for hook removal.
			std::memcpy(Localhook.Savedtext, Address, 14);
			Localhook.Function = *(Signature *)Address;
			Localhook.Savedlocation = Address;
			Localhook.Savedtarget = Target;

			// Write the opcodes.
			auto Protection = Memprotect::Unprotectrange(Address, 14);
			{
#if defined(ENVIRONMENT64)
				std::memcpy(Address, "\xFF\x25", 2);
				std::memcpy(Address + 2, "\x00\x00\x00\x00", 4);
				std::memcpy(Address + 6, &Target, sizeof(void *));
#else
				std::memcpy(Address, "\xE9", 1);
				Target = (uint8_t *)((size_t)Target - (size_t)Address - 5);
				std::memcpy(Address + 1, &Target, sizeof(void *));
#endif
			}
			Memprotect::Protectrange(Address, 14, Protection);

			return Localhook;
		}
		static Stomphook Install(std::uintptr_t Location, std::uintptr_t Target, bool Followjumps = true)
		{
			return Install(reinterpret_cast<void *>(Location), reinterpret_cast<void *>(Target), Followjumps);
		}
		static Stomphook Install(void *Location, std::uintptr_t Target, bool Followjumps = true)
		{
			return Install(Location, reinterpret_cast<void *>(Target), Followjumps);
		}
		static Stomphook Install(std::uintptr_t Location, void *Target, bool Followjumps = true)
		{
			return Install(reinterpret_cast<void *>(Location), Target, Followjumps);
		}
	};

	// Replace a block of data with NOP instructions.
	template <size_t Size>
	struct NOPPatch
	{
		uint8_t Savedtext[Size]{};
		void *Savedlocation;

		void Remove()
		{
			auto Protection = Memprotect::Unprotectrange(Savedlocation, Size);
			{
				std::memcpy(Savedlocation, Savedtext, Size);
			}
			Memprotect::Protectrange(Savedlocation, Size, Protection);
		}
		static NOPPatch Install(void *Location)
		{
			NOPPatch<Size> Localpatch;

			// Save the current information for patch removal.
			std::memcpy(Localpatch.Savedtext, Location, Size);
			Localpatch.Savedlocation = Location;

			auto Protection = Memprotect::Unprotectrange(Location, Size);
			{
				std::memset(Location, 0x90, Size);
			}
			Memprotect::Protectrange(Location, Size, Protection);

			return Localpatch;
		}
		static NOPPatch Install(std::uintptr_t Location)
		{
			return Install(reinterpret_cast<void *>(Location));
		}
	};

	
}


namespace Memprotect
{
#if defined(_WIN32)

	// Windows memory protection,
	inline void Protectrange(void *Address, const size_t Length, unsigned long Oldprotection)
	{
		unsigned long Temp;
		VirtualProtect(Address, Length, Oldprotection, &Temp);
	}
	inline unsigned long Unprotectrange(void *Address, const size_t Length)
	{
		unsigned long Oldprotection;
		VirtualProtect(Address, Length, PAGE_EXECUTE_READWRITE, &Oldprotection);
		return Oldprotection;
	}

#else

	// Nix memory protection.
	inline void Protectrange(void *Address, const size_t Length, unsigned long Oldprotection)
	{
		int Pagesize = getpagesize();
		*(size_t *)&Address -= size_t(Address) % Pagesize;
		mprotect(Address, Pagesize, Oldprotection);
	}
	inline unsigned long Unprotectrange(void *Address, const size_t Length)
	{
		unsigned long Oldprotection = (PROT_READ | PROT_WRITE | PROT_EXEC);

		// Get the old protection of the range, we assume it's continuous.
		std::FILE *Filehandle = std::fopen("/proc/self/maps", "r");
		if (Filehandle)
		{
			char Buffer[1024]{}, Permissions[5]{}, Device[6]{}, Mapname[256]{};
			unsigned long Start, End, Node, Foo;

			while (std::fgets(Buffer, 1024, Filehandle))
			{
				std::sscanf(Buffer, "%lx-%lx %4s %lx %5s %ld %s", &Start, &End, Permissions, &Foo, Device, &Node, Mapname);

				if (Start <= (unsigned long)Address || End >= (unsigned long)Address)
				{
					Oldprotection = 0;

					if (Permissions[0] == 'r') Oldprotection |= PROT_READ;
					if (Permissions[1] == 'w') Oldprotection |= PROT_WRITE;
					if (Permissions[2] == 'x') Oldprotection |= PROT_EXEC;

					break;
				}
			}

			std::fclose(Filehandle);
		}

		// Write the new protection.
		int Pagesize = getpagesize();
		*(size_t *)&Address -= size_t(Address) % Pagesize;
		mprotect(Address, Pagesize, PROT_READ | PROT_WRITE | PROT_EXEC);
		return Oldprotection;
	}

#endif
}
*/


PIMAGE_NT_HEADERS xImageNTHeader(uint64_t ImgBase);
size_t GetIATAddress(const char* ModuleName, const char* FunctionName, uint64_t ImgBase);
size_t WriteIATAddress(const char* ModuleName, const char* FunctionName, uint64_t ImgBase, void *NewProc);
size_t RedirectIAT(const char *Modulename, const char *Functionname, void *NewFunction);

#define QCALL(a, v, AddCall) Hook::Quick::Call((void*)(a), (uint32_t)(v), (AddCall))
#define QPATCH_JUMP Hook::Quick::CallPatch::PATCH_JUMP
#define QPATCH_CALL Hook::Quick::CallPatch::PATCH_CALL
#define QNOP(a, v) Hook::Quick::NOP((void*)(a), (v))
#define QMOV(a, v) *(BYTE*)a = 0xB8; *(DWORD*)(a + 1) = v;
#define PATCH_WINSOCK_IAT(name, function) \
    PatchCount += RedirectIAT("wsock32.dll", name, function) != 0 ? 1 : 0; \
    PatchCount += RedirectIAT("WS2_32.dll", name, function)  != 0 ? 1 : 0; 

#define PATCH_ntdll_IAT(name, function) RedirectIAT("ntdll.dll", name, function);
#define PATCH_KERNELBASE_IAT(name, function) RedirectIAT("KERNELBASE.dll", name, function);
#define PATCH_KERNEL32_IAT(name, function) RedirectIAT("KERNEL32.dll", name, function);

class Hook
{
public:
	class Stomp
	{
	public:
		uint8_t _OriginalCode[5];
		uint8_t _ByteCount;
		uint8_t *_Place;
		void *_Hook;
		bool _Jump;

		void Initialize(uintptr_t Place, void *HookToInstall = NULL, uint8_t ByteCount = 5, bool useJump = true);
		void InstallHook(void *HookToInstall = NULL);
		void ReleaseHook();
		static void PermanentHook(uintptr_t Place, void *HookToInstall);
	};
	class Quick
	{
	public:
		enum class CallPatch
		{
			PATCH_CALL,
			PATCH_JUMP,
			PATCH_NOTHING
		};
		static void Patch(void *Address, uint64_t Data, uint32_t ByteCount);
		static void NOP(void *Address, uint32_t Length);
		static void Call(void *Address, uint32_t Data, CallPatch PatchType);
	};
	class Call
	{
	public:
		uint8_t _OriginalCode[5];
		uint8_t *_Place;
		void *_Original;
		void *_Hook;

		void Initialize(uint32_t Place, void *HookToInstall = NULL);
		void InstallHook(void *HookToInstall = NULL);
		void ReleaseHook();
		static void PermanentHook(uint32_t Place, void *HookToInstall);
	};
};

/*
Usage example:

Hooking::Stomphook<decltype(std::memcpy)> Stomphook;
void *Stompfunc(void *Src, void *Dst, size_t Size)
{
void *Result{};

printf("Copying %llX bytes!\n", Size);
Callhook_ret(Stomphook, Result, Src, Dst, Size);

return Result;
}

Stomphook = Hooking::Stomphook<decltype(std::memcpy)>::Install(std::memcpy, Stompfunc);
Patch = Hooking::NOPPatch<sizeof(std::memcpy)>::Install(std::memcpy);
*/
