#pragma once

class BGIOObject
{
public:
	BGIOObject() : m_nRef(1), m_nTimerRef(0), m_nGeneralRef(1) {}
	virtual ~BGIOObject() {}

			void	AddRef();
			void	AddRef(LONG volatile* pRef);

			void	Release();
			void	Release(LONG volatile* pRef);
	
	virtual void	OnFree();
	
	/** 타이머 동작을 등록합니다. */
			void	AddTimer(ULONGLONG n64Time, int nId = 0);

	virtual void	OnWaitCallback() {}

	/** 타이머에 의해 실행됩니다. */
	virtual void	OnTimerCallback(int nId = 0) {}

	/** IOCP객체에 의해 실행됩니다. */
	virtual void	OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);

	/** OnTimerCallback에서 호출됩니다. */
	virtual void	OnTimer(int nId) {}

public:
	long	m_nRef;

	long	m_nTimerRef;
	
	long	m_nGeneralRef;
};

