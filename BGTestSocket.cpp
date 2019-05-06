#include "stdafx.h"
#include "BGTestSocket.h"

#include "BGTestServer.h"
#include "BGMainConfig.h"
#include "BGProtocal.h"

#include "BGGameObject.h"
#include "BGTestPlayer.h"

#pragma warning( push )
#pragma warning( disable : 4996 )

#define PENDING_WRITE				65535
#define PACKET_RESERVED_LENGTH		3

int							BGTestSocket::s_nNetworkId;
BGIOCompletionHandler*		BGTestSocket::s_pIOCPHandler;
long						BGTestSocket::s_nId;

BGTestSocket::BGTestSocket(long nId, SOCKET socket, sockaddr_in* addr)
	:BGIOSocket(socket)
{
	m_nId					= nId;
	m_nBit					= SOCKET_BIT_NOT_USED;
	if (addr != nullptr) {
		m_nAddr = addr->sin_addr;
		m_nPort = addr->sin_port;
	}	
	m_timeLogin				= time(0);

	m_pGameObject			= nullptr;
}
BGTestSocket::BGTestSocket()
	:BGIOSocket(INVALID_SOCKET)
{
	m_nId = 0;
	m_nBit = SOCKET_BIT_NOT_USED;
	m_timeLogin = 0;
	m_nWorldId = 1;
	m_pGameObject = nullptr;
}

BGTestSocket::~BGTestSocket()
{
}

void BGTestSocket::OnCreate()
{
	BG_LOG_INFO("new connect : in_addr(%s), index(%d)", inet_ntoa(m_nAddr), m_nId);

	BGIOSocket::OnCreate();
}

void BGTestSocket::OnClose()
{
	BG_LOG_INFO("close connection : in_addr(%s), port(%d), index(%d)", inet_ntoa(m_nAddr), m_nPort, m_nId);
	
	// 로그아웃 처리

	Lock();
	BitReset(SOCKET_BIT_CONNECTED);
	m_nId = 0;
	m_timeLogin = 0;

	BGTestPlayer* pPlayer = static_cast<BGTestPlayer*>(m_pGameObject);
	if (pPlayer)
		m_pGameObject = nullptr;

	BitSet(0);
	BitSet(SOCKET_BIT_NOT_USED);
	Unlock();

	if (pPlayer) {
		//pPlayer->Realease;
	}
}

bool BGTestSocket::CheckResetData()
{
	if ((m_nId != 0) || !BitIs(SOCKET_BIT_NOT_USED) || m_pGameObject != nullptr) {

		BG_LOG_ERROR("Not Reset Data. m_nId=%d, m_nBit=%0x, m_pObject=%d", m_nId, m_nBit, m_pGameObject);
		return false;
	}
	
	return true;
}

void BGTestSocket::ResetDataOnForce()
{
	BG_LOG_INFO("Reset Data. m_nId=%d", m_nId);

	m_nBit = SOCKET_BIT_NOT_USED;
	m_nId = 0;
	m_timeLogin = 0;
	m_pGameObject = nullptr;
}


void BGTestSocket::CloseSocket()
{
	SOCKET hSocket = InterlockedExchange((LONG*)&m_hSocket, 0);
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 0;
	setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));
	closesocket(hSocket);
}

void BGTestSocket::OnRead()
{
	if (m_nPendingWrite > PENDING_WRITE) {
		BG_LOG_ERROR("close pending socket : %s %d", inet_ntoa(m_nAddr), m_nPendingWrite);
		Close();
		return;
	}

	DWORD dwSize = m_pReadBuf->m_dwSize;
	char* szBuffer = m_pReadBuf->m_buffer;
	
	for (; ; )
	{
		if (dwSize < PACKET_RESERVED_LENGTH)
		{
			Read(dwSize);
			return;
		}
		packet_basic_protocal* packet = reinterpret_cast<packet_basic_protocal*>(szBuffer);
		PacketType nType = packet->type;
		PacketSize nSize = packet->size;

		if (dwSize < nSize) {
			if (nSize > BUFFER_SIZE) {
				BG_LOG_ERROR("exceed buffer size. : type=%d, size=%d, bit:%X", nType, nSize, m_nBit);
				Close();
			}
			else {
				Read(dwSize);
			}
			return;
		}
		else if (nSize < PACKET_RESERVED_LENGTH) {
			BG_LOG_ERROR("invalid buffer size. : type=%d, size=%d, bit:%X", nType, nSize, m_nBit);
			Close();
			return;
		}

		if (!Process(packet)) {
			BG_LOG_ERROR("Invalid state at BGSocket::Process : type=%d, Bit=%X, Addr=%s, Port=%d"
				, nType, m_nBit, inet_ntoa(m_nAddr), m_nPort);
		}

		dwSize -= nSize;
		szBuffer += nSize;
	}
}

bool BGTestSocket::Process(packet_basic_protocal* clientpacket)
{
	PacketType type = clientpacket->type;

	switch (type)
	{
	case PacketType::CS_PingTest:
	{
		cs_packet_ping_test* packet = reinterpret_cast<cs_packet_ping_test*>(clientpacket);
		
		BGIOBuffer* pSendBuffer = BGIOBuffer::Alloc();
		sc_packet_ping_test* sc_packet = reinterpret_cast<sc_packet_ping_test*>(pSendBuffer->m_buffer);
		sc_packet->size = sizeof(sc_packet_ping_test);
		sc_packet->type = PacketType::SC_PingTest;
		pSendBuffer->m_dwSize = sc_packet->size;
		Send(pSendBuffer);		

		
	}
		break;

	case PacketType::CS_Login:
	{
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(clientpacket);
				
		std::string nickName{ packet->strId };
		
		LoginOn(0, nickName);		
	}
		break;

	default:
	{
		BG_LOG_ERROR("Invalid packet type : type=%d, Addr=%s, Port=%d", type, inet_ntoa(m_nAddr), m_nPort);
	}
		return false;
	}
	
	BG_LOG_DEBUG("packet process success : type=%d", type);
	return true;
}

void BGTestSocket::Send(BGIOBuffer* pBuffer, bool bAlloc)
{
	if (bAlloc) {
		BGIOBuffer* pNewBuffer = BGIOBuffer::Alloc();
		memcpy(pNewBuffer->m_buffer, pBuffer->m_buffer, pBuffer->m_dwSize);
		pNewBuffer->m_dwSize = pBuffer->m_dwSize;
		BGIOSocket::Write(pBuffer);
	}
	else {
		BGIOSocket::Write(pBuffer);
	}
}

void BGTestSocket::LoginOn(__int64 n64UID, std::string nickName)
{
	Lock();

	if (!BitIs(SOCKET_BIT_CONNECTED)) {
		Unlock();
		return;
	}

	BGGameObject* pPlayerOld = m_pGameObject;
	m_pGameObject = new BGTestPlayer{m_nId, nickName, this};
	
	BitSet(SOCKET_BIT_LOGIN);

	BGIOBuffer* pSendBuffer = BGIOBuffer::Alloc();
	sc_packet_login* sc_packet = reinterpret_cast<sc_packet_login*>(pSendBuffer->m_buffer);
	sc_packet->size = sizeof(sc_packet_login);
	sc_packet->type = PacketType::SC_Login;
	sc_packet->client_id = m_nId;
	sc_packet->object_id = m_pGameObject->GetId();
	pSendBuffer->m_dwSize = sc_packet->size;
	Send(pSendBuffer);

		

	BitSet(SOCKET_BIT_LOADING);
	RequestDataLoad();
	BitReset(SOCKET_BIT_LOADING);
	
	LoadComplete();

	Unlock();
}

void BGTestSocket::Logout(bool bKickIs)
{
}

void BGTestSocket::RequestDataLoad()
{

}

void BGTestSocket::LoadComplete()
{
	BGIOBuffer* pSendBuffer = BGIOBuffer::Alloc();
	sc_packet_put_object* sc_packet = reinterpret_cast<sc_packet_put_object*>(pSendBuffer->m_buffer);
	sc_packet->size = sizeof(sc_packet_put_object);
	sc_packet->type = PacketType::SC_Put_Object;
	sc_packet->object_id = m_pGameObject->m_nId;
	sc_packet->object_type = static_cast<unsigned char>(ObjectType::PLAYER);
	sc_packet->x = m_pGameObject->GetPosition().x;
	sc_packet->y = m_pGameObject->GetPosition().y;
	pSendBuffer->m_dwSize = sc_packet->size;
	Send(pSendBuffer);
}

void BGTestSocket::SetViewList()
{
	m_pGameObject->m_VLLock.lock();
	m_pGameObject->m_setView.clear();
	m_pGameObject->m_VLLock.unlock();

	//BGTestServer::FindPlayer()

}

#pragma warning( pop )