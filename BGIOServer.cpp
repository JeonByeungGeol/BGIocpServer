#include "stdafx.h"
#include "BGIOServer.h"

#include "BGIOCompletionHandler.h"
#include "BGIOSocket.h"
#include "BGIOAcceptThread.h"

BGIOServer::BGIOServer()
{
	m_hSocket = INVALID_SOCKET;
	m_hAcceptEvent = WSA_INVALID_EVENT;
	m_pAcceptThread = nullptr;
	m_pIOCPHandler = nullptr;
}


BGIOServer::~BGIOServer()
{
	Close();
}

/**
 * 소켓 생성 -> Bind -> Listen 이후에
 * AcceptEvent 생성, eventSelect 시작, AcceptThread 생성 후 동작합니다.
*/
bool BGIOServer::Start(BGIOCompletionHandler * pIOCPHandler, int nPort)
{
	m_pIOCPHandler = pIOCPHandler;
	
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET) {
		BG_LOG_ERROR("invalid socket");
		return false;
	}

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(nPort);

	if (bind(m_hSocket, (sockaddr*)&sin, sizeof(sin))) {
		BG_LOG_ERROR("bind error");
		Close();
		return false;
	}

	if (listen(m_hSocket, 5)) {
		BG_LOG_ERROR("listen error");
		Close();
		return false;
	}
	
	m_hAcceptEvent = WSACreateEvent();
	WSAEventSelect(m_hSocket, m_hAcceptEvent, FD_ACCEPT);

	m_pAcceptThread = new BGIOAcceptThread{ m_hAcceptEvent, this };
	m_pAcceptThread->Begin(nullptr);

	return true;
}

/**
 * 소켓 종료, AcceptEvent 제거
*/
void BGIOServer::Close()
{
	if (m_hSocket != INVALID_SOCKET) {
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
	if (m_hAcceptEvent != WSA_INVALID_EVENT) {
		WSACloseEvent(m_hAcceptEvent);
		m_hAcceptEvent = WSA_INVALID_EVENT;
	}
}

/**
 * 소켓 종료
*/
void BGIOServer::Stop()
{
	SOCKET hSocket = m_hSocket;
	m_hSocket = INVALID_SOCKET;
	closesocket(hSocket);
}

/**
 * AcceptEvent에 신호가 들어왔을 때
 * AccetpThread에서 해당 함수가 호출되고, IOCP객체에 전달합니다.
 * 직접 처리하지 않고 WorkerThread에서 처리를 맡기는 이유는 스레드 풀을 관리해주는 IOCP의 기능을 이용하기 위함입니다.
*/
void BGIOServer::OnWaitCallback()
{
	WSAResetEvent(m_hAcceptEvent);
	m_pIOCPHandler->Post(0, this);
}

/**
 * IOCP객체에서 작업이 들어오면 해당함수가 호출됩니다.
 * accept를 진행하고, 
 * 소켓의 Initialize를 호출 한 후,
 * 후처리 작업(CreateSocket함수)을 진행합니다.
*/
void BGIOServer::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
	struct sockaddr_in clientAddress;
	int clientAddressLength = sizeof(clientAddress);
	SOCKET newSocket = accept(m_hSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
	if (newSocket == INVALID_SOCKET) {

	}

	BGIOSocket *pSocket = CreateSocket(newSocket, &clientAddress);
	if (pSocket == nullptr) {
		closesocket(newSocket);
		return;
	}
	pSocket->Initialize(m_pIOCPHandler);
	pSocket->Release();
}
