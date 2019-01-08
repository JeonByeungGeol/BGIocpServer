// BGStressTest.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"

#include "BGDummyClient.h"
#include "BGThreadFunc.h"



int main()
{
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	BGDummyClient::Init();
	
	std::vector<std::thread*> workerThreads;
	for (int i = 0; i < WORKER_THREAD_NUM; i++)
		workerThreads.push_back(new std::thread{ WorkerThread });

	std::thread connectThresds{ ConnectThread };
	connectThresds.join();

	Sleep(1000);

	std::thread pingTestThreads{ PingTestThread };

	while (1)
	{
		Sleep(1000);
	}

	for (auto th : workerThreads) {
		th->join();
		delete th;
	}

	WSACleanup();

    return 0;
}

