#include "stdafx.h"
#include "BGDummyClient.h"

#pragma warning (disable:4996)

HANDLE BGDummyClient::s_hIocp;
SOCKADDR_IN BGDummyClient::s_ServerAddr;
char BGDummyClient::s_ip[20];
std::vector<BGDummyClient*> BGDummyClient::s_Clients;

void BGDummyClient::Init()
{
	strcpy_s(s_ip, 20, "127.0.0.1");
	s_ServerAddr.sin_family = AF_INET;	
	s_ServerAddr.sin_addr.s_addr = inet_addr(s_ip);
	s_ServerAddr.sin_port = htons(SERVER_PORT);

	s_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	for (int i = 0; i < TEST_CLIENT_NUM; i++) {
		s_Clients.push_back(new BGDummyClient{});
	}
}

BGDummyClient::BGDummyClient()
{
	m_nBit = 0;
}


BGDummyClient::~BGDummyClient()
{
}

void BGDummyClient::BitSet(int nBit)
{
	InterlockedExchange(&m_nBit, m_nBit | nBit);
}

void BGDummyClient::BitReset(int nBit)
{
	InterlockedExchange(&m_nBit, m_nBit & ~nBit);
}

bool BGDummyClient::BitIs(int nBit)
{
	return m_nBit & nBit;
}

bool BGDummyClient::Connect(int id)
{
	m_hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	HANDLE hIocp = CreateIoCompletionPort((HANDLE)m_hSocket, s_hIocp, id, 0);

	if (WSAConnect(m_hSocket, (SOCKADDR*)&s_ServerAddr, sizeof(s_ServerAddr), NULL, NULL, NULL, NULL)
		== SOCKET_ERROR) {
		std::cout << "WSAConnect() Error!!" << std::endl;
		return false;
	}
	BitSet(CLIENT_BIT_CONNECT);
	return true;
}

bool BGDummyClient::Disconnect()
{
	if (m_hSocket) {
		closesocket(m_hSocket);
	}
	BitReset(CLIENT_BIT_CONNECT);
	return true;
}

bool BGDummyClient::SendPacket(char * packet, WORD size)
{
	SEND_STRUCT* send = new SEND_STRUCT;

	CopyMemory(send->m_SendBuffer, packet, size);
	send->m_SendWSABuf.buf = send->m_SendBuffer;
	send->m_SendWSABuf.len = size;

	ZeroMemory(&send->m_OverExSend, sizeof(OVERLAPPED_EX));
	send->m_OverExSend.m_io_type = IO_SEND;

	DWORD sendBytes;
	int result = WSASend(m_hSocket, &send->m_SendWSABuf, 1, &sendBytes, 0
		, reinterpret_cast<LPWSAOVERLAPPED>(&send->m_OverExSend), NULL);
	if (result == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			printf("send() error!!\n");
			return FALSE;
		}
	}
	
	return true;
}




