#include "stdafx.h"
#include "BGThreadFunc.h"

#include "BGDummyClient.h"

void WorkerThread()
{

}

void ConnectThread()
{
	DWORD flag = 0;

	for (int i = 0; i < TEST_CLIENT_NUM; i++) {
		BGDummyClient* pClient = g_Client[i];

		if (pClient->Connect(i)) {			
			ZeroMemory(&pClient->m_overExRecv, sizeof(OVERLAPPED_EX));
			pClient->m_overExRecv.m_io_type = IO_RECV;
			pClient->m_uncomplete_size = 0;
			pClient->m_current_packet_size = 0;

			pClient->m_recv_wsa_buf.buf = pClient->real_recv_buf;
			pClient->m_recv_wsa_buf.len = MAX_BUFF_SIZE;

			int result = WSARecv(pClient->GetSocket(), &pClient->m_recv_wsa_buf, 1
				, NULL, &flag, reinterpret_cast<LPWSAOVERLAPPED>(&pClient->m_overExRecv.m_Overlapped), NULL);

			if (result == SOCKET_ERROR) {
				int err = WSAGetLastError();
				if (err != WSA_IO_PENDING) {
					printf("WSARecv() error!!\n");
					return;
				}
			}
		}

		std::cout << "all connect" << std::endl;
	}
}


void PintTestThread()
{
	cs_packet_ping_test packet;
	packet.size = sizeof(cs_packet_ping_test);

	while (1)
	{
		for (int i = 0; i < TEST_CLIENT_NUM; i++) {
			g_Client[i]->SendPacket(reinterpret_cast<char*>(&packet), sizeof(packet));
		}
		Sleep(1000);
	}
}