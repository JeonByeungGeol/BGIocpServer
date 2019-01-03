#include "stdafx.h"
#include "BGTestSocket.h"



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
}

BGTestSocket::~BGTestSocket()
{
}

void BGTestSocket::OnCreate()
{
	BG_LOG_INFO("new connect : %hs", inet_ntoa(m_nAddr));
}

void BGTestSocket::OnClose()
{
}

void BGTestSocket::CloseSocket()
{
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
