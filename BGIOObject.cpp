#include "stdafx.h"
#include "BGIOObject.h"
#include "BGIOTimerThread.h"

void BGIOObject::AddRef()
{
	AddRef(&m_nGeneralRef);
}

void BGIOObject::AddRef(LONG volatile * pRef)
{
	if (InterlockedIncrement(&m_nRef) == 1) {
		BG_LOG_ERROR("");
		// EBREAK(); ���� StackWorker ���
		m_nRef = 100;
	}

	InterlockedIncrement(pRef);
}

void BGIOObject::Release()
{
	Release(&m_nGeneralRef);
}

void BGIOObject::Release(LONG volatile * pRef)
{
	long nRef = InterlockedDecrement(pRef);
	if (nRef < 0) {
		BG_LOG_ERROR("[pRef %p %lu %lld %d]"
		, this, *(DWORD*)this, (char*)pRef - (char*)this, nRef);
		// EBREAK(); ���� StackWorker ���
		*pRef = 100;
		m_nRef = 100;
	}

	nRef = InterlockedDecrement(&m_nRef);
	if (nRef > 0)
		return;
	if (nRef < 0) {
		BG_LOG_ERROR("[m_nRef %p %lu %d]"
			, this, *(DWORD*)this, nRef);
		// EBREAK(); ���� StackWorker ���
		return;
	}

	if (m_nTimerRef > 0 || m_nGeneralRef > 0) {
		BG_LOG_ERROR("[remain %p %lu (%d,%d,%d)]"
			, this, *(DWORD*) this, nRef, m_nTimerRef, m_nGeneralRef );
		// EBREAK(); ���� StackWorker ���
		m_nRef = 100;
		return;
	}



	OnFree();
}

void BGIOObject::OnFree()
{
	delete this;
}

void BGIOObject::AddTimer(ULONGLONG n64Time, int nId)
{
	AddRef(&m_nTimerRef);
	BGIOTimerThread::AddTimer(this, n64Time, nId);
}

void BGIOObject::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
	OnTimer(dwTransferred);
	Release(&m_nTimerRef);
}
