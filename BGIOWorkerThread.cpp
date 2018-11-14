#include "stdafx.h"
#include "BGIOWorkerThread.h"
#include "BGIOCompletionHandler.h"
#include "BGIOObject.h"

long				BGIOWorkerThread::s_nRunningThread;

BGIOWorkerThread::BGIOWorkerThread()
{
}


BGIOWorkerThread::~BGIOWorkerThread()
{
}


void BGIOWorkerThread::Run()
{
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
}


void BGIOWorkerThread::OnTerminate()
{
	HANDLE hEvent = CreateEvent(nullptr, false, false, nullptr);
	if (hEvent == nullptr)
	{
		BG_LOG_ERROR("Create Event Fail");
		return;
	}
	
	//BGIOTerminate Terminate;
	//Terminate.PostObject(m_pIOCPHandler, hEvent);

	WaitForSingleObject(hEvent, INFINITE);
	CloseHandle(hEvent);
}