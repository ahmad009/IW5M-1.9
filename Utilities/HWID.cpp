#include "..\Stdafx.h"
#define _WIN32_DCOM
#include <comdef.h>
#include <comutil.h>
#include <wbemidl.h>

using namespace std;
# pragma comment(lib, "wbemuuid.lib")

std::string keylog;
std::string GetLove()
{
	return md5(keylog);
	//return keylog;
}

string remove_space(string& str)
{
	int len = str.length();
	int j = 0;

	for (int i = 0; i < len;)
	{
		if (str.at(i) == ' ')
		{
			i++;
			continue;
		}

		str.at(j++) = str.at(i++);
	}
	str.resize(j);
	return str;
}

string lovely()
{
	string resultsn = "fail";
	string fail = "fail";

	HRESULT hres;


	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		std::cout << "Failed to initialize COM library. Error code = 0x"
			<< std::hex << hres << std::endl;
		return fail;
	}



	hres = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL
	);


	if (FAILED(hres))
	{
		std::cout << "Failed to initialize security. Error code = 0x"
			<< std::hex << hres << std::endl;
		CoUninitialize();
		return fail;
	}



	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		std::cout << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< std::hex << hres << std::endl;
		CoUninitialize();
		return fail;
	}


	IWbemServices *pSvc = NULL;


	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		&pSvc
	);

	if (FAILED(hres))
	{
		std::cout << "Could not connect. Error code = 0x"
			<< std::hex << hres << std::endl;
		pLoc->Release();
		CoUninitialize();
		return fail;
	}



	hres = CoSetProxyBlanket(
		pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
	);

	if (FAILED(hres))
	{
		std::cout << "Could not set proxy blanket. Error code = 0x"
			<< std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return fail;

	}


	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_DiskDrive"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		std::cout << "Query for operating system name failed."
			<< " Error code = 0x"
			<< std::hex << hres << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return fail;
	}


	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		bool foundtag = false;

		// Get the value of the Name property
		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		_bstr_t a = vtProp.bstrVal;

		if (strstr((char*)a, "PHYSICALDRIVE0"))
		{
			foundtag = true;
		}


		hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
		_bstr_t b = vtProp.bstrVal;

		if (foundtag)
		{
			resultsn = (char*)b;
			foundtag = false;
		}


		VariantClear(&vtProp);


		pclsObj->Release();
	}


	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return resultsn;
}

void Auth_VerifyIdentity();

void Authenticate()
{
	keylog = lovely();
	remove_space(keylog);
	Auth_VerifyIdentity();
}