#include "..\stdafx.h"
#include <shlwapi.h>
#include <ws2tcpip.h>

unsigned int oneAtATimeHash(char *key)
{
	unsigned int hash, i;
	size_t len = strlen(key);
	for (hash = i = 0; i < len; ++i)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

hostent* WINAPI custom_gethostbyname(const char* name)
{
	unsigned int ip1 = oneAtATimeHash("mw3-pc-auth.prod.demonware.net");
	unsigned int log1 = oneAtATimeHash("mw3-pc-lobby.prod.demonware.net");

	unsigned int stun1 = oneAtATimeHash("mw3-stun.us.demonware.net");
	unsigned int stun2 = oneAtATimeHash("mw3-stun.eu.demonware.net");

	unsigned int current = oneAtATimeHash((char*)name);
	char* hostname = (char*)name;


	if (current == ip1 || current == log1)
		hostname = MASTER_SERVER; 

	/*if (strstr(name, "stun"))
		hostname = "127.0.0.1";*/

	//Log::Debug("%s redirected to %s", name, hostname);
	//Log::Debug("custom_gethostbyname", name);

	return gethostbyname(hostname);
}

void PatchIAT()
{
	uint32_t PatchCount = 0;
	PATCH_WINSOCK_IAT("gethostbyname", custom_gethostbyname);
	/*PATCH_WINSOCK_IAT("connect", dw_connect);
	PATCH_WINSOCK_IAT("recv", dw_recv);
	PATCH_WINSOCK_IAT("recvfrom", dw_recvfrom);
	PATCH_WINSOCK_IAT("select", dw_select);
	PATCH_WINSOCK_IAT("send", dw_send);
	PATCH_WINSOCK_IAT("sendto", dw_sendto);*/
	//PATCH_WINSOCK_IAT("closesocket", dw_closesocket);
}
