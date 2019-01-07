#include "stdafx.h"
#include "BGTestSocket.h"

#include "BGTestServer.h"
#include "BGMainConfig.h"
#include "BGProtocal.h"

#pragma warning( push )
#pragma warning( disable : 4996 )

#define PENDING_WRITE				65535
#define PACKET_RESERVED_LENGTH		12

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
	BG_LOG_DEBUG("packet process test!");

	return true;
}

void BGTestSocket::Write()
{
}

void BGTestSocket::Send()
{
}

#pragma warning( pop )