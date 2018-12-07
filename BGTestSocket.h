#pragma once

#include "BGIOSocket.h"

class BGTestPlayer;

class BGTestSocket : public BGIOSocket
{
public:
	static	int							s_nNetworkId;
	static	BGIOCompletionHandler*		s_pIOCPHandler;

public:
	BGTestSocket(SOCKET pSocket, sockaddr_in* addr) : BGIOSocket(pSocket) {};
	~BGTestSocket();

	/** */
	virtual	void				OnClose() {};

	/** ReadCallback 에서 호출*/
	virtual	void				OnRead() {};


	long						Id() { return m_nId; }

	void						Lock() { m_lock.Enter(); }
	void						Unlock() { m_lock.Leave(); }


protected:
	long m_nId;

public:
	BGTestPlayer* m_pPlayer;
};

