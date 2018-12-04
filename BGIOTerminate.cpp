#include "stdafx.h"
#include "BGIOTerminate.h"
#include "BGIOCompletionHandler.h"

void BGIOTerminate::PostObject(BGIOCompletionHandler * pIOCPHandler, std::thread* pThread)
{
	m_pThread = pThread;
	pIOCPHandler->Post(0, this);
}

void BGIOTerminate::OnIOCallback()
{
	SuspendThread(m_pThread->native_handle());
	BG_LOG_INFO("thread join");
}
