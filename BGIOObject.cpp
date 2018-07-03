#include "stdafx.h"
#include "BGIOObject.h"


void BGIOObject::AddRef()
{
	AddRef(&m_nGeneralRef);
}

void BGIOObject::AddRef(LONG volatile * pRef)
{
	if (InterlockedIncrement(&m_nRef) == 1) {
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
		*pRef = 100;
		m_nRef = 100;
	}

	nRef = InterlockedDecrement(&m_nRef);
	if (nRef > 0)
		return;
	if (nRef < 0) {

	}

	if (m_nTimerRef > 0 || m_nGeneralRef > 0) {
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

}

void BGIOObject::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
	OnTimer(dwTransferred);
	Release(&m_nTimerRef);
}
