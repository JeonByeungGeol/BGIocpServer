#pragma once

class BGIOObject
{
public:
	BGIOObject() : m_nRef(1), m_nTimerRef(0), m_nGeneralRef(1) {}
	virtual ~BGIOObject() {}

	void AddRef();
	void AddRef(LONG volatile* pRef);

	void Release();
	void Release(LONG volatile* pRef);
	
	virtual void OnFree();

	virtual void OnWaitCallback() {}

	void AddTimer(ULONGLONG n64Time, int nId = 0);
	virtual void OnTimerCallback(int nId = 0) {}
	virtual void OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);
	virtual void OnTimer(int nId) {}
	


public:
	long m_nRef;

	long m_nTimerRef;
	
	long m_nGeneralRef;
};

