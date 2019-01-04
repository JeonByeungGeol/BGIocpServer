#include "stdafx.h"
#include "BGTestSocket.h"

#include "BGTestServer.h"
#include "BGMainConfig.h"

int							BGTestSocket::s_nNetworkId;
BGIOCompletionHandler*		BGTestSocket::s_pIOCPHandler;


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
	SOCKET hSocket = InterlockedExchange64((LONGLONG*)&m_hSocket, 0);
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 0;
	setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));
	closesocket(hSocket);
}

void BGTestSocket::OnRead()
{
}

bool BGTestSocket::Process()
{
	return false;
}

void BGTestSocket::Write()
{
}

void BGTestSocket::Send()
{
}
