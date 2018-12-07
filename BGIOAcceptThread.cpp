#include "stdafx.h"
#include "BGIOAcceptThread.h"

#include "BGIOCompletionHandler.h"
#include "BGIOObject.h"

BGIOAcceptThread::BGIOAcceptThread(HANDLE hAcceptEvent, BGIOObject* pServer)
	: m_pObject(pServer)
{
	m_vHandle[0] = hAcceptEvent;
	m_vHandle[1] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}


BGIOAcceptThread::~BGIOAcceptThread()
{
	CloseHandle(m_vHandle[1]);
}

void BGIOAcceptThread::Run()
{
	BG_LOG_DEBUG("AcceptThread Run On");
	for ( ; ;) {
		DWORD dwWaitResult = WaitForMultipleObjects(2, m_vHandle, FALSE, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0)
			m_pObject->OnWaitCallback();
		else
			break;
	}
	BG_LOG_DEBUG("AcceptThread Run Off");
}

void BGIOAcceptThread::OnTerminate()
{
	SetEvent(m_vHandle[1]);
	m_pThread->join();
	BG_LOG_DEBUG("AcceptThread Terminate");
}
