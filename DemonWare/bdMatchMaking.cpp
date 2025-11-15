#include "StdInc.h"
#include "DW.h"
#include "bdLSGServer.h"

#include <time.h>

char *base64_encode(const unsigned char *data,
	size_t input_length,
	size_t *output_length);
unsigned char *base64_decode(const char *data,
	size_t input_length,
	size_t *output_length);

class MatchMakingInfo : public bdResult
{
public:
	std::string address;
	uint32_t unknown1;
	uint32_t unknown2;

	// only for serialization
	std::string sessionID;
	uint32_t players;


	// IW5
	int Unknown1;
	int Playlist;
	int PlaylistVersion;
	int Unknown4;
	int Unknown5;
	int Unknown6;
	int Unknown7;
	int Unknown8;
	int Unknown9;
	int UnknownA;
	int UnknownB;
	std::string UnknownBlob;
	int UnknownC;
	uint32_t UnknownUInt1;
	uint32_t UnknownUInt2;
	float UnknownFloat1;
	float UnknownFloat2;
	int UnknownD;
	int UnknownE;
	int UnknownF;
	int UnknownG;
	int UnknownH;
	int UnknownI;
	int UnknownJ;
	int UnknownK;
	int UnknownL;
	int UnknownM;
	int UnknownN;
	int UnknownO;
	int UnknownP;
	int UnknownQ;
	int UnknownR;
	int UnknownS;
	uint64_t UnknownUInt64;

	// T5-specific
	/*
	int netcodeVersion;
	std::string unknownBlob2;
	char hostname[260];
	uint64_t unknownLong1;
	char mapname[260];
	int gametype;
	int playlist;
	int unknown6;
	int unknown7;
	int unknown8;
	uint32_t licenseType;
	int unknownA;
	int unknownB;
	int maxPlayers;
	int unknownD;
	uint64_t onlineID;
	int wager;
	int unknownF;
	char unknownString3[260];
	uint64_t unknownLong3;
	uint32_t statDDL;*/

	MatchMakingInfo()
	{
		unknown1 = 0;
		unknown2 = 0;

		players = 0;
	}

	virtual void deserialize(bdByteBuffer& buffer)
	{
		deserialize(buffer, false);
	}

	virtual void deserialize(bdByteBuffer& buffer, bool clientPart)
	{
		buffer.readBlob(address);

		if (clientPart)
		{
			buffer.readBlob(sessionID);
		}

		buffer.readUInt32(&unknown1);
		buffer.readUInt32(&unknown2);

		if (clientPart)
		{
			buffer.readUInt32(&players);
		}
		buffer.readInt32(&Unknown1);
		buffer.readInt32(&Playlist);
		buffer.readInt32(&PlaylistVersion);
		buffer.readInt32(&Unknown4);
		buffer.readInt32(&Unknown5);
		buffer.readInt32(&Unknown6);
		buffer.readInt32(&Unknown7);
		buffer.readInt32(&Unknown8);
		buffer.readInt32(&Unknown9);
		buffer.readInt32(&UnknownA);
		buffer.readInt32(&UnknownB);
		buffer.readBlob(UnknownBlob);
		buffer.readInt32(&UnknownC);
		buffer.readUInt32(&UnknownUInt1);
		buffer.readUInt32(&UnknownUInt2);
		buffer.readFloat(&UnknownFloat1);
		buffer.readFloat(&UnknownFloat2);
		buffer.readInt32(&UnknownD);
		buffer.readInt32(&UnknownE);
		buffer.readInt32(&UnknownF);
		buffer.readInt32(&UnknownG);
		buffer.readInt32(&UnknownH);
		buffer.readInt32(&UnknownI);
		buffer.readInt32(&UnknownJ);
		buffer.readInt32(&UnknownK);
		buffer.readInt32(&UnknownL);
		buffer.readInt32(&UnknownM);
		buffer.readInt32(&UnknownN);
		buffer.readInt32(&UnknownO);
		buffer.readInt32(&UnknownP);
		buffer.readInt32(&UnknownQ);
		buffer.readInt32(&UnknownR);
		buffer.readInt32(&UnknownS);
		buffer.readUInt64(&UnknownUInt64);



		/*// T5
		buffer.readInt32(&netcodeVersion);
		buffer.readBlob(unknownBlob2);
		buffer.readString(hostname, sizeof(hostname));
		buffer.readUInt64(&unknownLong1);
		buffer.readString(mapname, sizeof(mapname));
		buffer.readInt32(&gametype);
		buffer.readInt32(&playlist);
		buffer.readInt32(&unknown6);
		buffer.readInt32(&unknown7);
		buffer.readInt32(&unknown8);
		buffer.readUInt32(&licenseType);
		buffer.readInt32(&unknownA);
		buffer.readInt32(&unknownB);
		buffer.readInt32(&maxPlayers);
		buffer.readInt32(&unknownD);
		buffer.readUInt64(&onlineID);
		buffer.readInt32(&wager);
		buffer.readInt32(&unknownF);
		buffer.readString(unknownString3, sizeof(unknownString3));
		buffer.readUInt64(&unknownLong3);
		buffer.readUInt32(&statDDL);*/
	}

	virtual void serialize(bdByteBuffer& buffer)
	{
		serialize(buffer, false);
	}

	virtual void serialize(bdByteBuffer& buffer, bool clientPart)
	{
		buffer.writeBlob(address);

		if (!clientPart)
		{
			buffer.writeBlob(sessionID);
		}

		buffer.writeUInt32(unknown1);
		buffer.writeUInt32(unknown2);

		if (!clientPart)
		{
			buffer.writeUInt32(players);
		}

		buffer.writeInt32(Unknown1);
		buffer.writeInt32(Playlist);
		buffer.writeInt32(PlaylistVersion);
		buffer.writeInt32(Unknown4);
		buffer.writeInt32(Unknown5);
		buffer.writeInt32(Unknown6);
		buffer.writeInt32(Unknown7);
		buffer.writeInt32(Unknown8);
		buffer.writeInt32(Unknown9);
		buffer.writeInt32(UnknownA);
		buffer.writeInt32(UnknownB);
		buffer.writeBlob(UnknownBlob);
		buffer.writeInt32(UnknownC);
		buffer.writeUInt32(UnknownUInt1);
		buffer.writeUInt32(UnknownUInt2);
		buffer.writeFloat(UnknownFloat1);
		buffer.writeFloat(UnknownFloat2);
		buffer.writeInt32(UnknownD);
		buffer.writeInt32(UnknownE);
		buffer.writeInt32(UnknownF);
		buffer.writeInt32(UnknownG);
		buffer.writeInt32(UnknownH);
		buffer.writeInt32(UnknownI);
		buffer.writeInt32(UnknownJ);
		buffer.writeInt32(UnknownK);
		buffer.writeInt32(UnknownL);
		buffer.writeInt32(UnknownM);
		buffer.writeInt32(UnknownN);
		buffer.writeInt32(UnknownO);
		buffer.writeInt32(UnknownP);
		buffer.writeInt32(UnknownQ);
		buffer.writeInt32(UnknownR);
		buffer.writeInt32(UnknownS);
		buffer.writeUInt64(UnknownUInt64);

		/*// T5
		buffer.writeInt32(netcodeVersion);
		buffer.writeBlob(unknownBlob2);
		buffer.writeString(hostname);
		buffer.writeUInt64(unknownLong1);
		buffer.writeString(mapname);
		buffer.writeInt32(gametype);
		buffer.writeInt32(playlist);
		buffer.writeInt32(unknown6);
		buffer.writeInt32(unknown7);
		buffer.writeInt32(unknown8);
		buffer.writeUInt32(licenseType);
		buffer.writeInt32(unknownA);
		buffer.writeInt32(unknownB);
		buffer.writeInt32(maxPlayers);
		buffer.writeInt32(unknownD);
		buffer.writeUInt64(onlineID);
		buffer.writeInt32(wager);
		buffer.writeInt32(unknownF);
		buffer.writeString(unknownString3);
		buffer.writeUInt64(unknownLong3);
		buffer.writeUInt32(statDDL);*/
	}
};


class bdSessionId : public bdResult
{
public:
	uint64_t sessionID;

	bdSessionId(uint64_t sessionID)
		: sessionID(sessionID)
	{

	}

	virtual void deserialize(bdByteBuffer& buffer)
	{

	}

	virtual void serialize(bdByteBuffer& buffer)
	{
		buffer.writeBlob((char*)&sessionID, 8);
	}
};

PMSessionInfo LobbyToJoin;
void bdMatchMaking::updateNPInfo(MatchMakingInfo& info)
{
	sinfo.address = 0;
	sinfo.port = 3074;
	sinfo.maxplayers = 0;
	sinfo.players = 0;

	if (*(DWORD*)0x401B40 == 0x22E8A300)
	{
		sinfo.maxplayers = 2;
	}
	else
	{
		sinfo.maxplayers = 0;
	}


	//Trace("updatePMInfo", "dw_sec_kid: %s dw_sec_key: %s dw_serialized_addr: %s", sinfo.data.Get("dw_sec_kid"), sinfo.data.Get("dw_sec_key"), sinfo.data.Get("dw_serialized_addr"));

	PM_GetPMID(&sinfo.PMid);

	// hack to set the online id properly
	//info.onlineID = sinfo.PMid;

	bdByteBuffer minfo;
	info.serialize(minfo);

	size_t outLen;
	char* base64 = base64_encode(minfo.getBytes(), minfo.getLength(), &outLen);

	char base64Data[4096];
	memcpy(base64Data, base64, outLen);
	base64Data[outLen] = 0;

	sinfo.data.Set("mmInfo", base64Data);
	//sinfo.data.Set("statDDL", va("%d", info.statDDL));
	//sinfo.data.Set("netcodeVersion", va("%d", info.netcodeVersion));
	//sinfo.data.Set("serverType", va("%d", info.wager));
	free(base64);
}

void bdMatchMaking::createSession(bdServiceServer* server, bdByteBuffer& message)
{
	if (*(BYTE*)0x8B4B54 != 1)
	{

		auto reply = server->makeReply(0);
		reply->send();
		return;
	}

	MatchMakingInfo info;
	info.deserialize(message);

	updateNPInfo(info);

	auto async = PM_CreateSession(&sinfo);
	PMCreateSessionResult* result = async->Wait();

	if (result == nullptr)
	{
		auto reply = server->makeReply(2);
		reply->send();

		return;
	}

	npSID = result->sid;

	bdSessionId sessID(npSID);

	auto reply = server->makeReply(0);
	reply->addResult(&sessID);
	reply->send();
}

void bdMatchMaking::deleteSession(bdServiceServer* server, bdByteBuffer& message)
{
	std::string sid;
	message.readBlob(sid);

	PMSID pmSID = *(PMSID*)sid.c_str();
	
	auto async = PM_DeleteSession(npSID);

	if (async->Wait(5000) == nullptr)
	{
		auto reply = server->makeReply(2);
		reply->send();

		return;
	}

	auto reply = server->makeReply(0);
	reply->send();
}

void bdMatchMaking::updateSession(bdServiceServer* server, bdByteBuffer& message)
{
	std::string sid;
	uint32_t players;
	MatchMakingInfo info;

	message.readBlob(sid);
	info.deserialize(message);

	updateNPInfo(info);


	auto async =  PM_UpdateSession(npSID, &sinfo);

	if (async->Wait() == nullptr)
	{
		auto reply = server->makeReply(2);
		reply->send();

		return;
	}

	bdSessionId sessID(npSID);

	auto reply = server->makeReply(0);
	reply->addResult(&sessID);
	reply->send();
}

void bdMatchMaking::updateSessionPlayers(bdServiceServer* server, bdByteBuffer& message)
{
	std::string sid;
	uint32_t players;
	MatchMakingInfo info;

	message.readBlob(sid);
	message.readUInt32(&players);
	info.deserialize(message);

	updateNPInfo(info);

	auto async =  PM_UpdateSession(npSID, &sinfo);

	if (async->Wait(5000) == nullptr)
	{
		auto reply = server->makeReply(2);
		reply->send();

		return;
	}

	bdSessionId sessID(npSID);

	auto reply = server->makeReply(0);
	reply->addResult(&sessID);
	reply->send();
}



int e = 0;
int h = 0;
void bdMatchMaking::updateSessionsFromNP(bdServiceServer* server, PMDictionary& params)
{
	auto async = PM_RefreshSessions(params);

	if (async->Wait(5000) == nullptr)
	{
		auto reply = server->makeReply(2);
		reply->send();

		return;
	}

	PMID myID;
	PM_GetPMID(&myID);

	auto reply = server->makeReply(0);

	MatchMakingInfo minfos[100];

	int numSessions = PM_GetNumSessions();
	for (int i = 0; i < numSessions; i++)
	{
		PMSessionInfo info;
		PM_GetSessionData(i, &info);

		std::string mmInfo = info.data.Get("mmInfo");
		size_t outSize;

		unsigned char* buffer = base64_decode(mmInfo.c_str(), mmInfo.size(), &outSize);

		bdByteBuffer infoBuffer((char*)buffer, outSize);

		std::string sessID((char*)&info.sid, sizeof(info.sid));

		minfos[i].deserialize(infoBuffer, true);
		minfos[i].sessionID = sessID;

		//Trace("bdMatchmaking", "playlist is %i and got %i players in Lobby %d ", minfos[i].Playlist, minfos[i].players, (int)(info.PMid & 0xFFFFFFFF));
		if (*(DWORD*)0x401B40 == 0x22E8A300)
		{
			if (minfos[i].UnknownC == h)
			{
				uint64 lobbyid = 109212290963734528 + (int)(info.PMid & 0xFFFFFFFF);
				if (lobbyid != 109212290963734528 + (int)(myID & 0xFFFFFFFF))
				{
					Trace("bdMatchmaking", "+connect_lobby %llu\n", lobbyid);
					Cbuf_AddText(0, va("connect_lobby %llu\n", lobbyid));
					break;
				}

				//reply->addResult(&minfos[i]);
			}
		}
		else
		{
			if (minfos[i].Playlist == e)
			{
				uint64 lobbyid = 109212290963734528 + (int)(info.PMid & 0xFFFFFFFF);
				if (lobbyid != (109212290963734528 + (int)(myID & 0xFFFFFFFF)))
				{
					
					//Trace("bdMatchmaking", "+connect_lobby %llu\n", lobbyid);
					Cbuf_AddText(0, va("connect_lobby %llu\n", lobbyid));
					break;
				}

				//reply->addResult(&minfos[i]);
			}
		}
		

		free(buffer);
	}

	reply->send();
}

void bdMatchMaking::findSessionsPaged(bdServiceServer* server, bdByteBuffer& message)
{
	// base 'findSessionsPaged'
	uint32_t queryType;
	bool newPagingToken;
	std::string pagingToken;
	uint32_t resultsPerPage;

	message.readUInt32(&queryType);
	message.readBoolean(&newPagingToken);
	message.readBlob(pagingToken);
	message.readUInt32(&resultsPerPage);

	// geo query (type 6 in t5)
	uint32_t netcodeVersion;
	uint32_t unknown1;
	uint32_t serverType;
	uint32_t statDDL;
	uint32_t unknown2;
	uint32_t unknown3;
	uint32_t country;

	message.readUInt32(&netcodeVersion);
	message.readDataType(0x14); // NaN?!
	message.readUInt32(&serverType);

	// rest is unknown and we don't care about for now

	// set up a NP query
	PMDictionary params;
	//params.Set("statDDL", va("%d", statDDL));
	params.Set("netcodeVersion", va("%d", netcodeVersion));
	params.Set("serverType", va("%d", serverType));

	updateSessionsFromNP(server, params);
}

void bdMatchMaking::inviteToSession(bdServiceServer* server, bdByteBuffer& data)
{
	PMID onlineID;
	std::string sid;
	std::string attachment;

	data.readBlob(sid);
	data.readBlob(attachment);

	while (data.readUInt64(&onlineID))
	{
		uint8_t tempMsg[4096];
		tempMsg[0] = 3;
		memcpy(&tempMsg[1], sid.c_str(), 8);
		memcpy(&tempMsg[9], attachment.c_str(), attachment.size());

		PM_SendMessage(onlineID, (uint8_t*)tempMsg, attachment.size() + 9);
	}

	auto reply = server->makeReply(0);
	reply->send();
}

void bdMatchMaking::findSessions(bdServiceServer* server, bdByteBuffer& message)
{
	/*var queryType = packet.ByteBuffer.ReadUInt32(); // query type
	var vb = packet.ByteBuffer.ReadUInt32(); // seems to be 0
	var count = packet.ByteBuffer.ReadUInt32();
	var a = packet.ByteBuffer.ReadInt32();
    var e = packet.ByteBuffer.ReadInt32();*/
	uint32_t queryType;
	uint32_t vb;
	uint32_t count;
	int a;
	int b;
	int c;
	int d;
	int f;
	int g;

	if (*(DWORD*)0x401B40 == 0x22E8A300)
	{
		message.readUInt32(&queryType);
		message.readUInt32(&vb);
		message.readUInt32(&count);
		message.readInt32(&a);
		message.readInt32(&b);
		message.readInt32(&c);
		message.readInt32(&d);
		message.readInt32(&e);
		message.readInt32(&f);
		message.readInt32(&g);
		message.readInt32(&h);
	}
	else
	{
		message.readUInt32(&queryType);
		message.readUInt32(&vb);
		message.readUInt32(&count);
		message.readInt32(&a);
		message.readInt32(&e);
	}

	Trace("bdMatchmaking", "playlist is %i", e);
	//std::string sid;

	//message.readBlob(sid);

	//char sidStr[20];
	//_snprintf(sidStr, sizeof(sidStr), "%016llx", *(PMSID*)sid.c_str());

	PMDictionary params;
	//params.Set("sid", sidStr);

	updateSessionsFromNP(server, params);
}

void bdMatchMaking::findSessionFromID(bdServiceServer* server, bdByteBuffer& message)
{
	std::string sid;

	message.readBlob(sid);

	char sidStr[20];
	_snprintf(sidStr, sizeof(sidStr), "%016llx", *(PMSID*)sid.c_str());

	PMDictionary params;
	params.Set("sid", sidStr);

	updateSessionsFromNP(server, params);
}

void bdMatchMaking::get_performance_values(bdServiceServer* server, bdByteBuffer& data)
{
	auto reply = server->makeReply(0);
	reply->send();

}

void bdMatchMaking::findSessionsByEntityIDs(bdServiceServer* server, bdByteBuffer& data)
{
	std::vector<PMID> onlineIDs;
	PMID onlineID;

	while (data.readUInt64(&onlineID))
	{
		onlineIDs.push_back(onlineID);
	}

	std::string query = "[";
	bool first = true;

	for (auto& npID : onlineIDs)
	{
		query += va("%s\"%016llx\"", (first) ? "" : ", ", npID);

		first = false;
	}

	query += "]";

	PMDictionary params;
	params.Set("npid", query.c_str());

	updateSessionsFromNP(server, params);
}

void bdMatchMaking::callService(bdServiceServer* server, std::string& message)
{
	bdByteBuffer data(message);

	char subType = 0;
	data.readByte(&subType);
	Trace("bdMatchMaking", "call type (%i)", subType);
	//MessageBoxA(0,va("msg type %d",subType),"Kenny",MB_ICONINFORMATION);
	return;
	switch (subType)
	{
		case 1:
			createSession(server, data);
			break;
		case 2:
			updateSession(server, data);
			break;
		case 3:
			deleteSession(server, data);
			break;
		case 4:
			findSessionFromID(server, data);
			break;
		case 5:
			findSessions(server, data);
			break;
		case 8:
			inviteToSession(server, data);
			break;
		case 10:
			get_performance_values(server, data);
			break;
		case 12:
			updateSessionPlayers(server, data);
			break;
		case 13:
			findSessionsPaged(server, data);
			break;
		case 14:
			findSessionsByEntityIDs(server, data);
			break;
		default:
			Trace("bdMatchMaking", "unhandled type %i", subType);
			break;
	}
}