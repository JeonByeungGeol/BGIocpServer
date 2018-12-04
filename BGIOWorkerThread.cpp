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
	BGIOTerminate Terminate;
	Terminate.PostObject(m_pIOCPHandler, m_pThread);

	m_pThread->join();
	
}