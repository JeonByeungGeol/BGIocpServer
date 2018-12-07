#include "stdafx.h"
#include "BGIOWorkerThread.h"
#include "BGIOCompletionHandler.h"
#include "BGIOObject.h"
#include "BGIOTerminate.h"

long				BGIOWorkerThread::s_nRunningThread;

BGIOWorkerThread::BGIOWorkerThread()
{
}


BGIOWorkerThread::~BGIOWorkerThread()
{
}


void BGIOWorkerThread::Run()
{
	BG_LOG_DEBUG("WorkerThread Run On");
	SetThreadIdealProcessor(GetCurrentThread(), MAXIMUM_PROCESSORS);
	for (; ;)
	{
		DWORD dwTransferred{ 0 };
		BGIOObject* pObject{ nullptr };
		LPOVERLAPPED lpOverlapped{ nullptr };

		BOOL bSuccess = m_pIOCPHandler->Get(dwTransferred, &pObject, &lpOverlapped);
		InterlockedIncrement(&s_nRunningThread);
		pObject->OnIOCallback(bSuccess, dwTransferred, lpOverlapped);
		InterlockedDecrement(&s_nRunningThread);
	}
	BG_LOG_DEBUG("WorkerThread Run Off");
}


void BGIOWorkerThread::OnTerminate()
{
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hEvent == NULL)
	{
		BG_LOG_ERROR("Create Event Fail");
		return;
	}

	BGIOTerminate Terminate;
	Terminate.PostObject(m_pIOCPHandler, m_pThread, hEvent);
		
	// c++11에서 스레드를 강제종료하는 기능이 따로 없어,
	// native_handle을 얻어온 이후에 SuspendThread를 호출
	// 종료 확인은 SuspendThread호출 이후, event기능을 사용
	// -> std::thread join() 기능 대신 event를 사용
	WaitForSingleObject(hEvent, INFINITE);
	CloseHandle(hEvent);

	BG_LOG_DEBUG("WorkerThread Terminate");
}