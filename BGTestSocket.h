#pragma once

#include "BGIOSocket.h"
#include "BGGameObject.h"

class BGGameObject;
struct packet_basic_protocal;

#define SOCKET_BIT_DISCONNECTED		0x00000001
#define SOCKET_BIT_CONNECTED		0x00000002
#define SOCKET_BIT_LOGIN			0x00000004
#define SOCKET_BIT_LOADING			0x00000008
#define SOCKET_BIT_CLOSED			0x00000010

class BGTestSocket : public BGIOSocket
{
protected:
			

public:
	static	long						s_nId;
	static	int							s_nNetworkId;
	static	BGIOCompletionHandler*		s_pIOCPHandler;

			long						m_nId;
			long						m_nBit;
			in_addr						m_nAddr;
			int							m_nPort;
			time_t						m_timeLogin;
			BGGameObject*				m_pGameObject;
			bool						m_bClosed;

public:
	BGTestSocket(long nId, SOCKET pSocket, sockaddr_in* addr);
	BGTestSocket();
	virtual ~BGTestSocket();

			void						Lock() { m_lock.Enter(); }
			void						Unlock() { m_lock.Leave(); }

	virtual	void						OnCreate();
	virtual	void						OnClose();

/** BGTestServer::Stop에서 맵에 등록된 모든 소켓에 대해 호출*/
			void						CloseSocket();

	virtual	void						OnRead();
			bool						Process(packet_basic_protocal* clientpacket);
			void						Send(BGIOBuffer* pBuffer, bool bAlloc = false);
			
		
			long						Id() { return m_nId; }
			void						BitSet(int nBit) { InterlockedExchange(&m_nBit, m_nBit | nBit); }
			void						BitReset(int nBit) { InterlockedExchange(&m_nBit, m_nBit & ~nBit); }
			bool						BitIs(int nBit) { return ((m_nBit & nBit) == nBit) ?true : false; }
			 
			/** 로그인 성공*/
			void						LoginOn(__int64 n64UID, std::string nickName);
			void						Logout(bool bKickIs);
			
			/** 로그인 이후, 데이터 로드*/
			void						RequestDataLoad();

			/**데이터 로드가 완료된 이후 호출되고 로드된 데이터들을 클라이언트에 전송한다.*/
			void						LoadComplete();

			/** 필요한 데이터를 셋팅 (view list 등)*/
			void						SetViewList();
};

