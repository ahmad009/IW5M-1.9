#pragma once

class bdAuthServer : public bdServiceServer
{
public:
	bdAuthServer();

	virtual const char* getHostName()
	{
		return "mw3-pc-auth.prod.demonware.net";
	}
};

class bdSteamAuthService : public bdService
{
public:
	virtual uint8_t getType() { return 28; }

	virtual void callService(bdServiceServer* server, std::string& message);
};

class bdDediAuthService : public bdService
{
public:
	virtual uint8_t getType() { return 12; }

	virtual void callService(bdServiceServer* server, std::string& message);
};

class bdRegisterServer : public bdService
{
public:
	virtual uint8_t getType() { return 26; }

	virtual void callService(bdServiceServer* server, std::string& message);
};