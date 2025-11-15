// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once


#ifndef WIN32_LEAN_AND_MEAN
#define	WIN32_LEAN_AND_MEAN
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define	_CRT_SECURE_NO_WARNINGS
#endif


// Windows headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdint.h>

// C/C++ headers
#include <map>
#include <vector>
#include <list>
#include <stdarg.h>
#include <stdio.h>
#include <thread>
#include <direct.h>
#include <intrin.h>

#include <memory>

#include <shlobj.h>
#include <mmsystem.h>
#include <cstdint>
#include <fstream>

// OpenSteamWorks
#define NO_STEAM
#include "CCallback.h"
#include "ISteamClient011.h"
#include "ISteamContentServer002.h"
#include "ISteamUser016.h"
#include "ISteamFriends009.h"
#include "ISteamApps004.h"
#include "ISteamGameServer010.h"
#include "ISteamMasterServerUpdater001.h"
#include "ISteamMatchmaking008.h"
#include "ISteamNetworking005.h"
#include "ISteamRemoteStorage002.h"
#include "ISteamUtils005.h"
#include "ISteamUserStats010.h"
#include "ISteamMatchmakingServers002.h"

#include "Steam\SteamBase.h"

#include "Utilities\md5.h"
#include "Utilities\Patternscan.h"
#include "Utilities\Log.h"
#include "Utilities\Utils.h"
#include "Utilities\Hooking.h"

#include "Demonware\dw.h"
#include "Patches\DumpHandler.h"
#include "Patches\Authenticate.h"

#include "Patches\IW5.h"
#include "Patches\InGameConsole.h"
#include "PlusIW5.h"
#include "libpm.h"