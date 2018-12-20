#pragma once

#include "BGIOSocket.h"
#include "BGIOBuffer.h"

struct GAME_PACKET;
enum S2G_PROTOCOL;

class BGTestOtherSocket : public BGIOSocket
{
public:
	using TestSocketMap = std::map<int, BGTestOtherSocket*>;

	static	TestSocketMap				s_mapTestSocket;
	static	long						s_nPendingPacket;


	static	bool						Start();
	static	void						Stop(bool bAllStop);
	static	bool						Create(int nId, in_addr nAddr, int nPort);
	static	BGTestOtherSocket*				Find(long nId);
	static	char*						InfoPut(char* end, int& nCount);
	static	int							PendingAllWrite();
	static	void						WriteAll(BGTestOtherSocket* pPass, S2G_PROTOCOL nType, const char*, ...);


public:
	BGTestOtherSocket();
	virtual ~BGTestOtherSocket();

	void						Process(GAME_PACKET* gamepacket);
	void						Write(S2G_PROTOCOL nType, const char*, ...);
	void						Send(BGIOBuffer* pBuffer);

protected:
	virtual	void						OnClose();
	virtual	void						OnRead();




	int											m_nServerId;
};

