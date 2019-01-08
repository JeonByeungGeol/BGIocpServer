#include "stdafx.h"
#include "BGThreadFunc.h"

#include "BGDummyClient.h"

void WorkerThread()
{
	DWORD iosize;	// 이번에 받은 패킷 사이즈
	unsigned char* buf = NULL;	// 현재 조립중인 버퍼
	DWORD flag = 0;
	ULONG comp_key;
	OVERLAPPED_EX* over_ptr = NULL;
	int remained_io_data_size = 0;
	int result;

	while (1) {
		result = GetQueuedCompletionStatus(BGDummyClient::s_hIocp, &iosize, &comp_key, (LPOVERLAPPED*)&over_ptr, INFINITE);
		
		BGDummyClient* pClient = g_Client[comp_key];

		if (false == result) {
			closesocket(pClient->GetSocket());
			std::cout << comp_key << " client 오류 발생!" << std::endl;
			break;
		}

		if (0 == iosize) {
			closesocket(pClient->GetSocket());
			break;
		}

		if (IO_RECV == over_ptr->m_io_type) {
			remained_io_data_size = iosize;
			buf = reinterpret_cast<unsigned char*>(pClient->real_recv_buf);

			while (remained_io_data_size > 0) {
				if (0 == pClient->m_current_packet_size)
					pClient->m_current_packet_size = buf[0];

				int remained_packet_data_size = pClient->m_current_packet_size - pClient->m_uncomplete_size;
				if (remained_packet_data_size <= remained_io_data_size) {
					memcpy(pClient->m_uncomplete_packet + pClient->m_uncomplete_size
						, buf
						, remained_packet_data_size);
					pClient->ProcessPacket(reinterpret_cast<unsigned char*>(pClient->m_uncomplete_packet));

					pClient->m_uncomplete_size = 0;
					pClient->m_current_packet_size = 0;
					buf += remained_packet_data_size;
					remained_io_data_size += remained_packet_data_size;
				}
				else {
					memcpy(pClient->m_uncomplete_packet + pClient->m_uncomplete_size, buf, remained_io_data_size);
					pClient->m_uncomplete_size += remained_io_data_size;
					remained_io_data_size = 0;
				}
			}

			result = WSARecv(pClient->GetSocket(), &pClient->m_recv_wsa_buf, 1, NULL,
				&flag, reinterpret_cast<LPWSAOVERLAPPED>(&pClient->m_overExRecv), NULL);

			if (result == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					printf("WSARecv() error!!\n");
					return;
				}
			}

		}
		else if (IO_SEND == over_ptr->m_io_type) {
			delete over_ptr;
		}
		else {
			std::cout << "IO type error" << std::endl;
			break;
		}		
	}

	return;
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


void PingTestThread()
{
	cs_packet_ping_test packet;
	packet.size = sizeof(cs_packet_ping_test);

	while (1)
	{
		for (int i = 0; i < TEST_CLIENT_NUM; i++) {
			g_Client[i]->SendPacket(reinterpret_cast<char*>(&packet), sizeof(packet));
		}

		Sleep(5000);
	}
}