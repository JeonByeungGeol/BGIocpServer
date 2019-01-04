#pragma once

#include "BGIOSocket.h"

class BGTestPlayer;

#define SOCKET_BIT_LOGIN_REQUEST	0x00000001
#define SOCKET_BIT_LOGIN			0x00000002
#define SOCKET_BIT_LOADING			0x00000004
#define SOCKET_BIT_CLOSED			0x00000008

class BGTestSocket : public BGIOSocket
{
protected:
			long						m_nId;

public:
	static	long						s_nId;
	static	int							s_nNetworkId;
	static	BGIOCompletionHandler*		s_pIOCPHandler;

			long						m_nBit;
			in_addr						m_nAddr;
			int							m_nPort;
			time_t						m_timeLogin;
			BGTestPlayer*				m_pPlayer;
			bool						m_bClosed;

public:
	BGTestSocket(SOCKET pSocket, sockaddr_in* addr);
	virtual ~BGTestSocket();

			void						Lock() { m_lock.Enter(); }
			void						Unlock() { m_lock.Leave(); }

	virtual	void						OnCreate();
	virtual	void						OnClose();

/** BGTestServer::Stop에서 맵에 등록된 모든 소켓에 대해 호출*/
			void						CloseSocket();

	virtual	void						OnRead();
			bool						Process();
			void						Write();
			void						Send();

		
			long						Id() { return m_nId; }
			void						BitSet(int nBit) { InterlockedExchange(&m_nBit, m_nBit | nBit); }
			void						BitReset(int nBit) { InterlockedExchange(&m_nBit, m_nBit & ~nBit); }
};

