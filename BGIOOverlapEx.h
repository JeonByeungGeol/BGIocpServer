#pragma once

#include <WinSock2.h>

#define MAX_BUFF_SIZE 4096

enum class BGIOOperation{
	SEND,
	RECV,
	TIMER,
	END	// ������� ����
};

class BGIOOverlapEx
{
	// �뵵 (send, recv, timer ...)
	BGIOOperation operation;

	// origin overlap ����ü
	WSAOVERLAPPED original_overlap;
	
	// wsa ����
	WSABUF wsa_buf;

	unsigned char socket_buf[MAX_BUFF_SIZE];
	
	int packet_size;
};