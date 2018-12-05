// BGIocpServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "BGLayer.h"
#include "BGMainConfig.h"
#include "BGIOTimerThread.h"

#include <WinSock2.h>

// test
#include "BGTestTimerObject.h"

int main()
{
	WSADATA wsaData;
	int err = WSAStartup(0x0202, &wsaData);
	if (err)
	{
		return 0;
	}

	
	if (false == BGMainConfig::Open()) {
		std::cout << "MainConfig Open Fail" << std::endl;
		return -1;
	}

	if (false == g_LogManager.Start()) {
		std::cout << "LogManager Start Fail" << std::endl;
		return -1;
	}
	
	BGLayer::Add(BGMainConfig::s_nNumberOfThreads);


	
		
	BGIOTimerThread::Init();

	BG_LOG_INFO("Server Start");

	BGTestTimerObject* testTimerObj = new BGTestTimerObject{};
	BGIOTimerThread::AddTimer(testTimerObj, 1000, 0);
	BGIOTimerThread::AddTimer(testTimerObj, 5000, 1);
	BGIOTimerThread::AddTimer(testTimerObj, 10000, 2);




	for (; ;)
	{
		Sleep(1000);
	}

	BG_LOG_INFO("Server Stop");
	
	g_LogManager.Stop();

    return 0;
}

