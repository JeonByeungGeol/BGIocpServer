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
 * ���� ���� -> Bind -> Listen ���Ŀ�
 * AcceptEvent ����, eventSelect ����, AcceptThread ���� �� �����մϴ�.
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
 * ���� ����, AcceptEvent ����
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
 * ���� ����
*/
void BGIOServer::Stop()
{
	SOCKET hSocket = m_hSocket;
	m_hSocket = INVALID_SOCKET;
	closesocket(hSocket);
}

/**
 * AcceptEvent�� ��ȣ�� ������ ��
 * AccetpThread���� �ش� �Լ��� ȣ��ǰ�, IOCP��ü�� �����մϴ�.
 * ���� ó������ �ʰ� WorkerThread���� ó���� �ñ�� ������ ������ Ǯ�� �������ִ� IOCP�� ����� �̿��ϱ� �����Դϴ�.
*/
void BGIOServer::OnWaitCallback()
{
	WSAResetEvent(m_hAcceptEvent);
	m_pIOCPHandler->Post(0, this);
}

/**
 * IOCP��ü���� �۾��� ������ �ش��Լ��� ȣ��˴ϴ�.
 * accept�� �����ϰ�, 
 * ������ Initialize�� ȣ�� �� ��,
 * ��ó�� �۾�(CreateSocket�Լ�)�� �����մϴ�.
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
