#include "..\stdafx.h"

#define AS_LOCAL					0
#define AS_INTERNET					1
#define AS_FAVORITES				2
#define AS_HISTORY					3
#define AS_FRIENDS					4

typedef struct servernode_s {
	DWORD	addr; //0-4
	WORD	port;//4-5
	BYTE	pswrd;//6
	BYTE	pure;//7
	BYTE	con_disabled;//8
	BYTE	nettype;//9
	BYTE	clients;//10
	BYTE	maxclients;//11
	char	placeholder[8];//12-18
	WORD	voice;//20
	WORD	minping;//22
	WORD	maxping;//24
	WORD	ping;//26
	char	placeholder2[8];
	BYTE	dedicated;//36
	char	hostname[32];//37
	char	mapname[56];//69
	char	gametype[27];//125
	BYTE	servernum;//151
	char	placeholder3[4];
	BYTE	placeholder4;//155
	BYTE	placeholder5;//157
	char	sv_hostname[49];//159
} servernode_t;


DWORD ServerBaseAddress = (DWORD)0xC86B30;
DWORD ServerBaseCount = (DWORD)0xC86B1C;//0xC86B28
DWORD ServerBaseCount2 = (DWORD)0xC86B28;

void WriteServerInformation(DWORD memaddr, DWORD addr, int port, int pswrd, int con_disabled, int pure, int nettype, int clients, int maxclients, int voice, int minping, int maxping, int ping, int dedicated, char* hostname, char* mapname, char* gametype, int servernum, int unk, int unk2, char*	sv_hostname)
{
	//DWORD memaddr, 
	//Log::Error("", "%p", Server);

	static servernode_s item;

	item.addr = addr;
	item.port = port;
	item.pswrd = pswrd;
	item.con_disabled = con_disabled;
	item.pure = pure;
	item.nettype = nettype;
	item.clients = clients;
	item.maxclients = maxclients;
	item.voice = voice;
	item.minping = minping;
	item.maxping = maxping;
	item.ping = ping;
	item.dedicated = dedicated;
	strcpy(item.hostname, hostname);
	strcpy(item.mapname, mapname);
	strcpy(item.gametype, gametype);
	item.servernum = servernum;
	item.placeholder4 = unk;
	item.placeholder5 = unk2;
	strcpy(item.sv_hostname, sv_hostname);

	memaddr = memaddr + ((servernum - 1) * 208);

	memcpy((void*)memaddr, &item, sizeof(item));

	DWORD SVBaseCount = 0xC86B1C;//0xC86B28
	DWORD SVBaseCount2 = 0xC86B28;

	//*(BYTE*)SVBaseCount = servernum;
	//*(BYTE*)SVBaseCount2 = servernum;
}



#include "Wininet.h"
#pragma comment(lib, "Wininet.lib")


DWORD getMyIP()
{
	DWORD myIP;

	char szBuffer[1024];

#ifdef WIN32
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		return 0;
#endif


	if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
	{
#ifdef WIN32
		WSACleanup();
#endif
		return 0;
	}

	struct hostent *host = gethostbyname(szBuffer);
	if (host == NULL)
	{
#ifdef WIN32
		WSACleanup();
#endif
		return 0;
	}

	//Obtain the computer's IP
	BYTE A = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1;
	BYTE B = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2;
	BYTE C = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3;
	BYTE D = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4;

	myIP = (A & 0xFF) * 0x1000000 ^ (B & 0xFF) * 0x10000 ^ (C & 0xFF) * 0x100 ^ (D & 0xFF);

#ifdef WIN32
	WSACleanup();
#endif
	return myIP;
}


DWORD ExternalIP = 0;

DWORD GetExternalIP(bool bswap, char * local = NULL);

DWORD GetExternalIP(bool bswap, char * local)
{
	if (ExternalIP != 0 && local == NULL)
	{
		if (bswap)
			return htonl(ExternalIP);
		else
			return ExternalIP;
	}
	else
	{
		char ipbuf[256];
		DWORD fsize = 0;

		if (local != NULL)
		{
			fsize = strlen(local);
			if (fsize < 20)
				strcpy(ipbuf, local);
			else
				fsize = 0;
		}
		else
		{
			fsize = 0;
		}


		if ((fsize != 0) && (fsize <= 20))
		{
			//return local ip
			ExternalIP = ConvertIPSTRtoDWORD(ipbuf);
		}
		else
		{
			//return external ip
			ExternalIP = getMyIP();
			if (ExternalIP == 0) ExternalIP = 0x0100007F;//127.0.0.1
		}

		return GetExternalIP(bswap);
	}
}

DWORD HttpReadFile(char * url, char * buffer, DWORD buffer_size)
{
	memset(buffer, 0, buffer_size);
	DWORD dwBytesRead = 0;

	HINTERNET inet = InternetOpenA(va("MW3"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (inet <= 0)
	{
		return 0;
	}


	DWORD var = 4000L; // millisec
	int b = InternetSetOption(inet, INTERNET_OPTION_CONNECT_TIMEOUT, (void *)&var, sizeof(var));
	b = InternetSetOption(inet, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, (void *)&var, sizeof(var));
	b = InternetSetOption(inet, INTERNET_OPTION_DATA_SEND_TIMEOUT, (void *)&var, sizeof(var));
	b = InternetSetOption(inet, INTERNET_OPTION_SEND_TIMEOUT, (void	*)&var, sizeof(var));
	b = InternetSetOption(inet, INTERNET_OPTION_RECEIVE_TIMEOUT, (void *)&var, sizeof(var));
	var = 4; // Retries
	b = InternetSetOption(inet, INTERNET_OPTION_CONNECT_RETRIES, (void *)&var, sizeof(var));


	HINTERNET iurl = InternetOpenUrlA(inet, url, "", 0, INTERNET_FLAG_RELOAD, 0);
	if (iurl <= 0)
	{
		InternetCloseHandle(inet);
		return 0;
	}

	if (InternetReadFile(iurl, buffer, buffer_size, &dwBytesRead))
	{
		InternetCloseHandle(iurl);
		InternetCloseHandle(inet);
		return dwBytesRead;
	}

	return 0;
}

DWORD WINAPI IpDiscoveryThreadFunc(LPVOID lpParameter)
{
	int ret = 5;
	int fsize = 0;
	char ipbuf[256];

	while (ret != 0 && fsize == 0)
	{
		fsize = HttpReadFile("http://teknogods.com/ip.php", ipbuf, sizeof(ipbuf));
		ret--;
		Sleep(1);
	}

	if (fsize != 0 && fsize <= 20)
		GetExternalIP(false, ipbuf);
	else
		Log::Error("GetInternetIP", "Failed to get external ip from ip.php");


	return 0;
}

bool isLocalIP(DWORD ipaddress)
{
	bool result;

	if (GetExternalIP(true) == ipaddress)
		result = true;
	else
		result = false;
	/*WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD(1, 1);
	char *ip;

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
		if (gethostname(name, sizeof(name)) == 0)
		{
			printf("Host name: %s\n", name);

			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				int nCount = 0;
				while (hostinfo->h_addr_list[nCount])
				{
					ip = inet_ntoa(*(
						struct in_addr *)hostinfo->h_addr_list[nCount]);

					printf("IP #%d: %s\n", ++nCount, ip);
				}
			}
		}
*/
	return result;
}