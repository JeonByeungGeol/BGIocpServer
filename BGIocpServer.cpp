// BGIocpServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "BGLock.h"
#include "BGIOBuffer.h"

#include <WinSock2.h>
int main()
{
	WSADATA wsaData;
	int err = WSAStartup(0x0202, &wsaData);
	if (err)
	{
		return 0;
	}


	std::cout << "-----------------------main start-----------------------" << std::endl;
	g_Config.Load();
	g_LogManager.Start();

	std::cout << "------test start--------" << std::endl;

	std::cout << "--------test end----------" << std::endl;
	
	g_LogManager.Stop();

	std::cout << "-----------------------main end-----------------------" << std::endl;

    return 0;
}

