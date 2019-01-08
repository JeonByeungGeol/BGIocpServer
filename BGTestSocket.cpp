#include "stdafx.h"
#include "BGTestSocket.h"

#include "BGTestServer.h"
#include "BGMainConfig.h"
#include "BGProtocal.h"

#include "BGTestPlayer.h"

#pragma warning( push )
#pragma warning( disable : 4996 )

#define PENDING_WRITE				65535
#define PACKET_RESERVED_LENGTH		3

int							BGTestSocket::s_nNetworkId;
BGIOCompletionHandler*		BGTestSocket::s_pIOCPHandler;
long						BGTestSocket::s_nId;

BGTestSocket::BGTestSocket(SOCKET socket, sockaddr_in* addr)
	:BGIOSocket(socket)
{
	m_nId					= InterlockedIncrement( &s_nId);
	m_nBit					= 0;
	m_nAddr					= addr->sin_addr;
	m_nPort					= addr->sin_port;
	m_timeLogin				= time(0);

	m_pPlayer				= nullptr;
	m_bClosed				= false;
}

BGTestSocket::~BGTestSocket()
{
}

void BGTestSocket::OnCreate()
{
	BG_LOG_INFO("new connect : %s", inet_ntoa(m_nAddr));

	BGTestServer::Add(this);
	BGIOSocket::OnCreate();

	if (BGMainConfig::s_nMaxUser < BGTestServer::Size()) {
		// 접속 실패 패킷 전송
		// 로그아웃 처리
		GracefulClose();
	}
}

void BGTestSocket::OnClose()
{
	BG_LOG_INFO("close connection : in_addr(%s), port(%d), socket(%p)", inet_ntoa(m_nAddr), m_nPort, this);
	
	BGTestServer::Remove(this);

	// 로그아웃 처리

	Lock();
	BGTestPlayer* pPlayer = m_pPlayer;
	if (pPlayer)
		m_pPlayer = nullptr;
	Unlock();

	if (pPlayer) {
		//pPlayer->Realease;
	}

	m_bClosed = true;
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

	if (m_nBit & SOCKET_BIT_CLOSED) {
		Unlock();
		return;
	}

	BGTestPlayer* pPlayerOld = m_pPlayer;
	m_pPlayer = new BGTestPlayer{this, n64UID, nickName};
	
	BitSet(SOCKET_BIT_LOGIN);

	BGIOBuffer* pSendBuffer = BGIOBuffer::Alloc();
	sc_packet_login* sc_packet = reinterpret_cast<sc_packet_login*>(pSendBuffer->m_buffer);
	sc_packet->size = sizeof(sc_packet_login);
	sc_packet->type = PacketType::SC_Login;
	sc_packet->id = m_nId;
	pSendBuffer->m_dwSize = sc_packet->size;
	Send(pSendBuffer);

		

	BitSet(SOCKET_BIT_LOADING);
	RequestDataLoad();
	BitReset(SOCKET_BIT_LOADING);
	
	Unlock();
}

void BGTestSocket::Logout(bool bKickIs)
{
}

void BGTestSocket::RequestDataLoad()
{

}

#pragma warning( pop )