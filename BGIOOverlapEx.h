#pragma once

#include <WinSock2.h>

#define MAX_BUFF_SIZE 4096

enum class BGIOOperation{
	SEND,
	RECV,
	TIMER,
	END	// 사용하지 않음
};

class BGIOOverlapEx
{
	// 용도 (send, recv, timer ...)
	BGIOOperation operation;

	// origin overlap 구조체
	WSAOVERLAPPED original_overlap;
	
	// wsa 버퍼
	WSABUF wsa_buf;

	unsigned char socket_buf[MAX_BUFF_SIZE];
	
	int packet_size;
};