// ==========================================================
// T5M project
// 
// Component: client
// Sub-component: loopdw
// Purpose: Initialization/entry code for LoopDW
//
// Initial author: NTAuthority
// Started: 2013-04-14
// ==========================================================

#include "StdInc.h"
#include "DW.h"
#include "bdAuthServer.h"
#include "bdLSGServer.h"
#include <map>
#include <vector>
#include <string>
#include <unordered_map>

#include <process.h>

static std::unordered_map<std::string, bdServer*> _servers;
static std::unordered_map<int, bdServer*> _serverHashes;
static std::map<SOCKET, bdServer*> _sockets;

void dw_register_server(bdServer* server)
{
	_servers[server->getHostName()] = server;
}

hostent* WINAPI dw_gethostbyname(const char* name)
{

	std::unordered_map<std::string, bdServer*>::const_iterator i = _servers.find(name);

	if (i != _servers.end())
	{
		static in_addr dwAddr;
		dwAddr.s_addr = 0x7F000000 | (std::hash<std::string>()(name) & 0xFFFFFF);
		
		static in_addr* dwSockAddrList[2];
		dwSockAddrList[0] = &dwAddr;
		dwSockAddrList[1] = nullptr;

		static hostent dwHostEnt;
		dwHostEnt.h_name = const_cast<char*>(name);
		dwHostEnt.h_aliases = NULL;
		dwHostEnt.h_addrtype = AF_INET;
		dwHostEnt.h_length = sizeof(in_addr);
		dwHostEnt.h_addr_list = (char**)dwSockAddrList;

		_serverHashes[dwAddr.s_addr] = i->second;

		return &dwHostEnt;
	}

	if (Global::Game::Type == GameType::GAME_TYPE_DEDI)
	{
		return gethostbyname(name);
	}
	return gethostbyname(name);
}

int WINAPI dw_connect(SOCKET socket, const sockaddr* name, int namelen)
{
	if (namelen == sizeof(sockaddr_in))
	{
		sockaddr_in* addr = (sockaddr_in*)name;

		std::unordered_map<int, bdServer*>::const_iterator i = _serverHashes.find(addr->sin_addr.s_addr);

		if (i != _serverHashes.end())
		{
			_sockets[socket] = i->second;

			return 0;
		}
	}

	return connect(socket, name, namelen);
}

static bdServer* dw_lookup_socket(SOCKET socket)
{
	std::map<SOCKET, bdServer*>::const_iterator i = _sockets.find(socket);

	if (i != _sockets.end())
	{
		return i->second;
	}

	return nullptr;
}

int WINAPI dw_send(SOCKET socket, const char* buf, int len, int flags)
{
	bdServer* server = dw_lookup_socket(socket);

	if (server)
	{
		std::string packet(buf, len);
		server->deferredHandlePacket(packet);

		return len;
	}

	return send(socket, buf, len, flags);
}

int WINAPI dw_sendto(SOCKET socket, const char* buf, int len, int flags, const sockaddr* to, int tolen)
{
	bdServer* server = dw_lookup_socket(socket);

	if (server)
	{
		std::string packet(buf, len);
		server->deferredHandlePacket(packet);

		return len;
	}

	return sendto(socket, buf, len, flags, to, tolen);
}

int WINAPI dw_recv(SOCKET socket, char* buf, int len, int flags)
{
	bdServer* server = dw_lookup_socket(socket);

	if (!server)
	{
		return recv(socket, buf, len, flags);
	}

	if (server->isPacketAvailable())
	{
		return server->dequeuePacket(buf, len);
	}

	WSASetLastError(WSAEWOULDBLOCK);
	return SOCKET_ERROR;
}

int WINAPI dw_recvfrom(SOCKET socket, char* buf, int len, int flags, sockaddr* from, int* fromlen)
{
	bdServer* server = dw_lookup_socket(socket);

	if (!server)
	{
		return recvfrom(socket, buf, len, flags, from, fromlen);
	}

	if (server->isPacketAvailable())
	{
		return server->dequeuePacket(buf, len);
	}

	WSASetLastError(WSAEWOULDBLOCK);
	return SOCKET_ERROR;
}

int WINAPI dw_select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const timeval* timeout)
{
	std::vector<SOCKET> readSockets;
	std::vector<SOCKET> writeSockets;

	for (std::map<SOCKET, bdServer*>::const_iterator i = _sockets.begin(); i != _sockets.end(); i++)
	{
		if (readfds)
		{
			if (FD_ISSET(i->first, readfds))
			{
				readSockets.push_back(i->first);

				FD_CLR(i->first, readfds);
			}
		}

		if (writefds)
		{
			if (FD_ISSET(i->first, writefds))
			{
				writeSockets.push_back(i->first);
			}
		}

		if (exceptfds)
		{
			if (FD_ISSET(i->first, exceptfds))
			{
				FD_CLR(i->first, exceptfds);
			}
		}
	}

	int retval = select(nfds, readfds, writefds, exceptfds, timeout);

	for (int i = 0; i < readSockets.size(); i++)
	{
		FD_SET(readSockets.at(i), readfds);
		retval++;
	}

	for (int i = 0; i < writeSockets.size(); i++)
	{
		if (_sockets[writeSockets.at(i)]->isPacketAvailable())
		{
			FD_SET(writeSockets.at(i), writefds);
			retval++;
		}
	}

	return retval;
}

int WINAPI dw_closesocket(SOCKET s)
{
	bdServer* server = dw_lookup_socket(s);

	if (server)
	{
		_sockets.erase(s);
		return 0;
	}

	return closesocket(s);
}

void dw_thread(void* arg)
{
	while (true)
	{
		Sleep(1);

		for (std::map<SOCKET, bdServer*>::const_iterator i = _sockets.begin(); i != _sockets.end(); i++)
		{
			i->second->processQueues();
		}
	}
}

static bdServer* lsgServer;

//void HandleP2PIM(PMID from, const uint8_t* buf, uint32_t buflen);
void IW5Notify(const char* title, const char* message);

void dw_im_received(PMID from, const uint8_t* buf, uint32_t buflen)
{
	uint8_t type = *buf;
	if (type == 255 && buf[1] == 255)
	{
		IW5Notify("^5PlusCI", "Please wait a few seconds and reconnect again.\nIf you still had any problem on connecting restart your game and try again!");
		return;
	}

	dwMessage msg(lsgServer, 2, false);
	msg.byteBuffer.writeUInt32(21);
	msg.byteBuffer.writeUInt64(from);
	msg.byteBuffer.writeString("me");
	msg.byteBuffer.writeBlob((const char*)buf, buflen);
	msg.send(true);

	Trace("dwhandler", "received IM from %llx", from);

	return;
	/*
	uint8_t type = *buf;


	dwMessage msg(lsgServer, 2, false);

	if (type == 3)
	{
		msg.byteBuffer.writeUInt32(6);
	}
	else
	{
		msg.byteBuffer.writeUInt32(21);
	}
	
	msg.byteBuffer.writeUInt64(from);
	msg.byteBuffer.writeString("me");

	if (type == 3)
	{
		msg.byteBuffer.writeBlob((const char*)(&buf[1]), 8);
		msg.byteBuffer.writeBlob((const char*)(&buf[9]), buflen - 9);
	}
	else
	{
		msg.byteBuffer.writeBlob((const char*)buf + 1, buflen - 1);
	}

	msg.send(true);

	Trace("dwHandler", "received IM from %llx", from);*/
}

void dw_patch()
{
	*(DWORD*)0x7DC478 = (DWORD)dw_gethostbyname;
	*(DWORD*)0x7DC480 = (DWORD)dw_recvfrom;
	*(DWORD*)0x7DC484 = (DWORD)dw_sendto;
	*(DWORD*)0x7DC49C = (DWORD)dw_recv;
	*(DWORD*)0x7DC4A0 = (DWORD)dw_send;
	*(DWORD*)0x7DC4A4 = (DWORD)dw_connect;
	*(DWORD*)0x7DC4C0 = (DWORD)dw_select;
}

void dw_patch_ds()
{
	*(DWORD*)0x6D9380 = (DWORD)dw_gethostbyname;

	// DW stuff
	*(DWORD*)0x6D9378 = (DWORD)dw_sendto;
	*(DWORD*)0x6D93A0 = (DWORD)dw_recv;
	*(DWORD*)0x6D93A4 = (DWORD)dw_send;
	*(DWORD*)0x6D93C8 = (DWORD)dw_recvfrom;
	*(DWORD*)0x6D93A8 = (DWORD)dw_select;
	*(DWORD*)0x6D93BC = (DWORD)dw_connect;
}

dw_entry entry;
void dw_init()
{

	uint32_t PatchCount = 0;
	PATCH_WINSOCK_IAT("gethostbyname", dw_gethostbyname);
	PATCH_WINSOCK_IAT("connect", dw_connect);
	PATCH_WINSOCK_IAT("recv", dw_recv);
	PATCH_WINSOCK_IAT("recvfrom", dw_recvfrom);
	PATCH_WINSOCK_IAT("select", dw_select);
	PATCH_WINSOCK_IAT("send", dw_send);
	PATCH_WINSOCK_IAT("sendto", dw_sendto);
	dw_register_server(new bdAuthServer());
	
	lsgServer = new bdLSGServer();
	dw_register_server(lsgServer);

	// game-specific addresses, so we ask this of the game patch module

	PM_RegisterMessageCallback(dw_im_received);

	// start the main loop thread
	_beginthread(dw_thread, 0, NULL);
}