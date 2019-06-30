// BGIocpServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "BGLayer.h"
#include "BGMainConfig.h"
#include "BGIOTimerThread.h"

#include <WinSock2.h>

// test
#include "BGTestTimerObject.h"
#include "BGTestBasicMonster.h"

#include "BGTestServer.h"
#include "BGTestSocket.h"
#include "BGProtocal.h"

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
	
	BG_LOG_INFO("SERVER ON STARTED");
	BGTestSocket::s_nNetworkId = BGLayer::Add(BGMainConfig::s_nNumberOfThreads);
	BGTestSocket::s_pIOCPHandler = BGLayer::GetIOCP(BGTestSocket::s_nNetworkId);
	BGTestServer::Start(BGTestSocket::s_nNetworkId);		
	BGIOTimerThread::Init();		

	BG_LOG_INFO("SERVER ON COMPLETED");


	/**
	 * 테스트	
	*/
	Sleep(1000);
	BGTestTimerObject* testTimerObj = new BGTestTimerObject{};
	//BGIOTimerThread::AddTimer(testTimerObj, 1000, 0);
	//BGIOTimerThread::AddTimer(testTimerObj, 5000, 1);
	//BGIOTimerThread::AddTimer(testTimerObj, 10000, 2);

	auto point = (BGTestSocket*)0;

	BGTestBasicMonster* testObject = new BGTestBasicMonster(5);
	testObject->AddTimer(0, 50);

	// 10초 이후 종료
	for (; ;)
	{
		Sleep(1000);
	}


	BG_LOG_INFO("SHUTDOWN STARTED");
	BGTestServer::Stop();
	BGIOThread::Terminate();
	BGIOBuffer::FreeAll();
	BGLayer::Clear();
	BGMainConfig::Close();
	BG_LOG_INFO("SHUTDOWN COMPLETED");
	
	g_LogManager.Stop();

    return 0;
}

