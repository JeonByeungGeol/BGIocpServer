#pragma once

#include "BGIOSocket.h"
#include "BGGameObject.h"

class BGGameObject;
struct packet_basic_protocal;

#define SOCKET_BIT_NOT_USED			0x00000000
#define SOCKET_BIT_LOGIN			0x00000001
#define SOCKET_BIT_LOADING			0x00000002
#define SOCKET_BIT_CLOSED			0x00000004

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
			BGGameObject*				m_pGameObject;
			bool						m_bClosed;

public:
	BGTestSocket(SOCKET pSocket, sockaddr_in* addr);
	virtual ~BGTestSocket();

			void						Lock() { m_lock.Enter(); }
			void						Unlock() { m_lock.Leave(); }

	virtual	void						OnCreate();
	virtual	void						OnClose();

/** BGTestServer::Stop���� �ʿ� ��ϵ� ��� ���Ͽ� ���� ȣ��*/
			void						CloseSocket();

	virtual	void						OnRead();
			bool						Process(packet_basic_protocal* clientpacket);
			void						Send(BGIOBuffer* pBuffer, bool bAlloc = false);
			
		
			long						Id() { return m_nId; }
			void						BitSet(int nBit) { InterlockedExchange(&m_nBit, m_nBit | nBit); }
			void						BitReset(int nBit) { InterlockedExchange(&m_nBit, m_nBit & ~nBit); }

			/** �α��� ����*/
			void						LoginOn(__int64 n64UID, std::string nickName);
			void						Logout(bool bKickIs);
			
			/** �α��� ����, ������ �ε�*/
			void						RequestDataLoad();

			/**������ �ε尡 �Ϸ�� ���� ȣ��ǰ� �ε�� �����͵��� Ŭ���̾�Ʈ�� �����Ѵ�.*/
			void						LoadComplete();

			/** �ʿ��� �����͸� ���� (view list ��)*/
			void						SetViewList();
};

