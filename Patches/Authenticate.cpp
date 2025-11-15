#include "..\stdafx.h"
#include <Shlwapi.h>
#include <shellapi.h>

#define CURL_STATICLIB
#include <curl.h>
#include <easy.h>

int authUserID;
char authUsername[256];
char authObfuscatedUsername[256];
char authSessionID[40];
int userid;



size_t AuthDataReceived(void *ptr, size_t size, size_t nmemb, void *data)
{
	if ((strlen((char*)data) + (size * nmemb)) > 8192)
	{
		return (size * nmemb);
	}

	strncat((char*)data, (const char*)ptr, size * nmemb);
	return (size * nmemb);
}

void Auth_Error(const char* message)
{
	//->int btn;
	static wchar_t buf[32768];
	MultiByteToWideChar(CP_UTF8, 0, message, -1, buf, sizeof(buf));

	MessageBoxW(NULL, buf, L"Auth Error!", MB_OK | MB_ICONSTOP);
}

int OUserID()
{
	return userid;
}

char* Auth_GetUsername()
{
#ifdef NoAuth
	return "Ahmad009";
#endif
	return authUsername;
}

char* Auth_GetObfuscatedUsername()
{
	return authObfuscatedUsername;
}

char* Auth_GetSessionID()
{
#ifdef NoAuth
	return "loooooooooooooooooooooooool";
#endif
	return authSessionID;
}

char* Auth_GetCountry()
{
return "IR";
}

int Auth_GetUserID()
{
#ifdef NoAuth
	return 1337;
#endif
	return authUserID;
}

std::string GetLove();

bool Auth_ParseResultBuffer(const char* result)
{
	bool ok = false;
	static char buf[16384];
	strncpy(buf, result, sizeof(buf));

	int i = 0;
	char* tokens[16];
	char* tok = strtok(buf, "#");

	while (tok && (i < 16))
	{
		tokens[i++] = tok;
		tok = strtok(NULL, "#");
	}

	// ok
	if (tokens[0][0] == 'o')
	{
		ok = true;
	}

	if (!ok)
	{
		Auth_Error(tokens[1]);
	}
	else
	{
		authUserID = atoi(tokens[2]);
		userid = atoi(tokens[2]);
		strcpy(authSessionID, tokens[5]);
		strncpy(authUsername, tokens[3], sizeof(authUsername));

		strncpy(authObfuscatedUsername, authUsername, sizeof(authObfuscatedUsername));
		int size = strlen(authObfuscatedUsername);

		for (int i = 0; i < size; i++)
		{
			authObfuscatedUsername[i] ^= 0xd3;
		}


		strcpy((char*)0x7EFBF0, Auth_GetSessionID());
		strcpy((char*)0x7EFB50, GetLove().c_str());

	}

	return ok;
}

#include <atlstr.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

char SN[64];


DWORD GetPhysicalDriveSerialNumber(UINT nDriveNumber IN, CString& strSerialNumber OUT);

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);
unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);


void printSerialNumber(char* input, int size)
{
	bool next = false;
	bool started = false;
	for (int i = 0; i < size; i++)
	{
		if (!next)
		{
			if (input[i] != ' ')
			{

			}
			else
			{
				next = true;
				//printf("%c", input[i]);
				//break;
			}
		}
		else
		{
			if (input[i] != ' ')
			{
				sprintf(SN, "%s%c", SN, input[i]);
				started = true;
			}
			else
			{
				if (started)
				{
					break;
				}
			}
		}
	}
}

char* cmd1 = "d21pYyBwYXRoIHdpbjMyX3BoeXNpY2FsbWVkaWEgZ2V0IFNlcmlhbE51bWJlcixUYWc="; // wmic path win32_physicalmedia get SerialNumber,Tag
char* cmd2 = "UEhZU0lDQUxEUklWRTA="; //PHYSICALDRIVE0

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);

	if (!pipe) throw std::runtime_error("_popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != NULL)
		{
			result += buffer.data();
			if (strstr(buffer.data(), "PHYSICALDRIVE0"))
			{
				printSerialNumber(buffer.data(), buffer.size());
				//printf(buffer.data());
				//printf(output.c_str());
			}
		}
	}
	return result;
}

void GetSN()
{
	exec("wmic diskdrive get Name,SerialNumber");
}


bool Auth_PerformSessionLogin(const char* username, const char* password)
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURL* curl = curl_easy_init();

	if (curl)
	{
		char url[255];
		_snprintf(url, sizeof(url), "http://%s/remauth.php", "pmauth.ir");
		//_snprintf(url, sizeof(url), "http://%s/remauth.php", "127.0.0.1");
		//_snprintf(url, sizeof(url), "http://%s/remauth.php", "auth.iw4.prod.fourdeltaone.net:1337");

		char buf[8192] = { 0 };
		char postBuf[8192];
		//_snprintf(postBuf, sizeof(postBuf), "%s&&%s", username, password);


		/*char SN[65];
		CString nSerialNumber;
		DWORD dwRet = GetPhysicalDriveSerialNumber(0, nSerialNumber);
		sprintf(SN, "%s", nSerialNumber.Trim());*/
		GetSN();
		std::string temp(SN);
		/*char buff[255];
		std::string temp(GetLove());
		for (int i = 0; i<temp.size(); i++){
		buff[i] = tolower(temp.c_str()[i]);
		}
		std::string temp2(buff);*/
		_snprintf(postBuf, sizeof(postBuf), "%s&&%s&&%s", username, password, md5(temp).c_str());

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, AuthDataReceived);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&buf);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "PlusMW3");
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postBuf);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		curl_global_cleanup();

		if (code == CURLE_OK)
		{
			std::cout << buf << std::endl;
			return Auth_ParseResultBuffer(buf);
		}
		else
		{
			Auth_Error(va("Could not reach PlusMaster server. Error code from CURL: %x.", code));
		}

		return false;
	}

	curl_global_cleanup();
	return false;
}


#pragma comment(lib, "Wininet.lib")
#include <Wininet.h>
char* RequestInfoFromLauncher(char* Key)
{

	DeleteUrlCacheEntry("http://127.0.0.1:8010/GetAuthResult");
	curl_global_init(CURL_GLOBAL_ALL);

	CURL* curl = curl_easy_init();

	if (curl)
	{
		char url[255];
		_snprintf(url, sizeof(url), "http://%s/%s", "127.0.0.1:8010", Key);
		//_snprintf(url, sizeof(url), "http://%s/remauth.php", "127.0.0.1");
		//_snprintf(url, sizeof(url), "http://%s/remauth.php", "auth.iw4.prod.fourdeltaone.net:1337");

		char buf[8192] = { 0 };
		//_snprintf(postBuf, sizeof(postBuf), "%s&&%s", username, password);


		/*char SN[65];
		CString nSerialNumber;
		DWORD dwRet = GetPhysicalDriveSerialNumber(0, nSerialNumber);
		sprintf(SN, "%s", nSerialNumber.Trim());*/
		//GetSN();
		//std::string temp(SN);
		/*char buff[255];
		std::string temp(GetLove());
		for (int i = 0; i<temp.size(); i++){
		buff[i] = tolower(temp.c_str()[i]);
		}
		std::string temp2(buff);*/

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, AuthDataReceived);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&buf);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "PlusClient");
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		curl_global_cleanup();

		if (code == CURLE_OK)
		{
			std::cout << buf << std::endl;
			return buf;
		}
		else
		{
			Auth_Error(va("Please make sure PlusMater Client is updated and running. Error code from CURL: %x.", code));
			ExitProcess(0xDEAD);
		}

		return "0";
	}

	curl_global_cleanup();
	return "0";
}
std::string remove_space(std::string& str);

void Auth_VerifyIdentity()
{
	strcpy(authSessionID, remove_space(std::string(RequestInfoFromLauncher("GetSessionID"))).c_str());
	strncpy(authUsername, remove_space(std::string(RequestInfoFromLauncher("GetUserName"))).c_str(), sizeof(authUsername));

	authUserID = atoi(remove_space(std::string(RequestInfoFromLauncher("GetUserID"))).c_str());
	userid = atoi(remove_space(std::string(RequestInfoFromLauncher("GetUserID"))).c_str());

	strncpy(authObfuscatedUsername, authUsername, sizeof(authObfuscatedUsername));
	int size = strlen(authObfuscatedUsername);

	for (int i = 0; i < size; i++)
	{
		authObfuscatedUsername[i] ^= 0xd3;
	}

	return;
	/*
	authUserID = 5;
	userid = 5;
	strcpy(authSessionID, "abcefG7898765431234567890876543s");
	strncpy(authUsername, "Sa3id", sizeof(authUsername));

	strncpy(authObfuscatedUsername, authUsername, sizeof(authObfuscatedUsername));


	strcpy((char*)0x7EFBF0, Auth_GetSessionID());
	strcpy((char*)0x7EFB50, GetLove().c_str());
	return;*/

	ExitProcess(0x8000D3AD);
}

size_t UDataReceived(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t rsize = (size * nmemb);
	char* text = (char*)ptr;
	int version = atoi(text);

	if (version > BUILDNUMBER)
	{
		MessageBoxA(NULL, va("This version (%d) has expired. Please use PlusMaster Client to obtain a new version (%d).", BUILDNUMBER, version), "Error!", MB_OK);
		ExitProcess(0x1);
	}

	return rsize;
}

void DisableOldVersions()
{
	if (IsDebuggerPresent()) return;

	curl_global_init(CURL_GLOBAL_ALL);

	CURL* curl = curl_easy_init();

	if (curl)
	{
		char url[255];
		_snprintf(url, sizeof(url), "http://dl.plusmaster.pro/Games/ver1.9.txt");

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UDataReceived);
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&steamID);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "PlusMW3");
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		curl_global_cleanup();

		if (code == CURLE_OK)
		{
			return;
		}
		else
		{
			// TODO: set some offline mode flag
		}
	}

	curl_global_cleanup();
}

size_t DLCDataReceived(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t rsize = (size * nmemb);
	char* text = (char*)ptr;

	//ShellExecute(NULL, "open", text, 0, 0, SW_SHOWNORMAL);
	//MessageBoxA(0, text, "info", 0);
	return rsize;
}

void Auth_OpenDLCStore()
{
	ShellExecute(NULL, "open", "http://plusmaster.pro/", 0, 0, SW_SHOWNORMAL);
	return;
}

void BindStunPort(int port)
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURL* curl = curl_easy_init();

	if (curl)
	{
		char url[255];
		_snprintf(url, sizeof(url), "http://185.85.211.149:13060/bind/%d/", port);

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DLCDataReceived);
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&steamID);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "PlusMW3");
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		curl_global_cleanup();

		if (code == CURLE_OK)
		{
			return;
		}
		else
		{
			// TODO: set some offline mode flag
		}
	}

	curl_global_cleanup();
}



