#include "stdafx.h"
#include "BGIOTerminate.h"
#include "BGIOCompletionHandler.h"

void BGIOTerminate::PostObject(BGIOCompletionHandler * pIOCPHandler, std::thread* pThread, HANDLE hEvent)
{
	m_pThread = pThread;
	m_hEvent = hEvent;
	pIOCPHandler->Post(0, this);
}

void BGIOTerminate::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
	SetEvent(m_hEvent);
	SuspendThread(m_pThread->native_handle());	
}
