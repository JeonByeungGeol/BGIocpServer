#include "stdafx.h"
#include "BGLock.h"


#include <shared_mutex>
void test()
{
	std::shared_lock<std::shared_mutex> lock;
}

//------------------------------- ABCDE ==================================//
//----------------------------------------------------------------------
thread_local BGLockInfo tlsLockInfo; //C++11 ǥ�� TLS ����
BGRWLock BGILock::s_rwLock;

//------------------------------- LockInfo=================================//
BGLockInfo::BGLockInfo()
	: m_dwPendingLock(0), m_n64LastLockedTicks(0)
{
}

BGLockInfo::~BGLockInfo()
{
	BGILock::s_rwLock.WriteLock();

	BGILock::GetLockInfoMap().erase(this);

	BGILock::s_rwLock.WriteUnlock();
}
//----------------------------------------------------------------------


//------------------------------- BGILock ==================================//
// static ����Լ�
std::map<BGLockInfo*, BGLockInfo*>& BGILock::GetLockInfoMap()
{
	static std::map<BGLockInfo*, BGLockInfo*> s_mapLockList;
	return s_mapLockList;
}

// static ����Լ�
BOOL BGILock::IsDeadLock(int nLimitSec)
{
	s_rwLock.ReadLock();
	
	for (auto& it : GetLockInfoMap())
		if (it.second->m_dwPendingLock > 0
			&& (it.second->m_n64LastLockedTicks + nLimitSec * CLOCKS_PER_SEC) < ::GetTickCount64()) {
			return TRUE;
		}

	s_rwLock.ReadUnlock();

	return FALSE;
}

BGILock::BGILock()
{
	s_rwLock.WriteLock();

	GetLockInfoMap()[&tlsLockInfo] = &tlsLockInfo;

	s_rwLock.WriteUnlock();
}

void BGILock::IncPendingLock()
{
	InterlockedExchange(&tlsLockInfo.m_n64LastLockedTicks, ::GetTickCount64()); // //���� ����� �������� �ɸ� ���� ���� ������ �Ȼ���
	InterlockedIncrement(&tlsLockInfo.m_dwPendingLock);
}

void BGILock::DecPendingLock()
{
	InterlockedDecrement(&tlsLockInfo.m_dwPendingLock);
}
//----------------------------------------------------------------------




//------------------------------- BGIOSpinLock ==================================//
BGIOSpinLock::BGIOSpinLock()
	: m_nLock(0)
{
}

void BGIOSpinLock::Enter()
{
	IncPendingLock();

	if (InterlockedExchange(&m_nLock, 1))
		Wait();
}

void BGIOSpinLock::Leave()
{
	DecPendingLock();

	InterlockedExchange(&m_nLock, 0);
}

BOOL BGIOSpinLock::TryEnter()
{
	return InterlockedExchange(&m_nLock, 1) == 0;
}


void BGIOSpinLock::Wait()
{
	// _mm_pause()
	// ��Ƽ������ ���α׷��ֿ��� ���� ��� Ÿ�̹��� ª�ٸ� �̷������� spin lock�� ����Ͽ� busy waiting�� �ϴ� ���� ���ɿ� ���ٰ� �˷��� �ִ�.
	// ������ ������ spin lock���� ȿ���� ���� ���� ������, CPU�� �̸� ���� pause��� ��ɾ �����Ѵ�.
	// spin lock�̶�°� �ᱹ ������ ���� ���ϴ� ���� �ɶ����� ��� loop�� ������ ���ε�,
	// puase��� ��ɾ ���� CPU���� ���� spin loop�� ���� �ִٴ� ��Ʈ�� �ְ�,
	// CPU �� �޸𸮹��� �ӵ� ���� �����ؼ� ������ �ٲ�µ� �ʿ��� �ð���ŭ ���� ��ɾ �������� �ʴ� ���̴�.
	// https://software.intel.com/sites/default/files/m/d/4/1/d/8/17689_w_spinlock.pdf (w_spinlock.pdf)

	int count = 4000;
	while (--count >= 0) {
		if (InterlockedExchange(&m_nLock, 1) == 0)
			return;
		_mm_pause();
	}

	count = 4000;
	while (--count >= 0) {
		SwitchToThread();
		if (InterlockedExchange(&m_nLock, 1) == 0)
			return;
	}

	for (; ;) {
		Sleep(1000);
		if (InterlockedExchange(&m_nLock, 1) == 0)
			return;
	}
}

//----------------------------------------------------------------------




//------------------------------- BGRWLock ==================================//
BGRWLock::BGRWLock()
	:m_nCount(0)
	, m_nLock(0)
{
	m_hREvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hWEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

BGRWLock::~BGRWLock()
{
	CloseHandle(m_hREvent);
	CloseHandle(m_hWEvent);
}

void BGRWLock::Wait()
{
	// _mm_pause()
	// ��Ƽ������ ���α׷��ֿ��� ���� ��� Ÿ�̹��� ª�ٸ� �̷������� spin lock�� ����Ͽ� busy waiting�� �ϴ� ���� ���ɿ� ���ٰ� �˷��� �ִ�.
	// ������ ������ spin lock���� ȿ���� ���� ���� ������, CPU�� �̸� ���� pause��� ��ɾ �����Ѵ�.
	// spin lock�̶�°� �ᱹ ������ ���� ���ϴ� ���� �ɶ����� ��� loop�� ������ ���ε�,
	// puase��� ��ɾ ���� CPU���� ���� spin loop�� ���� �ִٴ� ��Ʈ�� �ְ�,
	// CPU �� �޸𸮹��� �ӵ� ���� �����ؼ� ������ �ٲ�µ� �ʿ��� �ð���ŭ ���� ��ɾ �������� �ʴ� ���̴�.
	// https://software.intel.com/sites/default/files/m/d/4/1/d/8/17689_w_spinlock.pdf (w_spinlock.pdf)

	 int count = 4000;
	while (--count >= 0) {
		if (InterlockedExchange(&m_nLock, 1) == 0)
			return;
		_mm_pause();
	}

	count = 4000;
	while (--count >= 0) {
		SwitchToThread();
		if (InterlockedExchange(&m_nLock, 1) == 0)
			return;
	}

	for (; ;) {
		Sleep(1000);
		if (InterlockedExchange(&m_nLock, 1) == 0)
			return;
	}
}

void BGRWLock::Enter()
{
	if (InterlockedExchange(&m_nLock, 1))
		Wait();
}

void BGRWLock::Leave()
{
	InterlockedExchange(&m_nLock, 0);
}

void BGRWLock::ReadLock()
{
	IncPendingLock();

	int nCount;

	nCount = InterlockedIncrement(&m_nCount);
	if (nCount & 0xff00)
	{
		Enter();
		nCount = m_nCount;
		do
		{

		} while ((nCount & 0xff00) != 0);

		if (nCount & 0xff0000)
		{
			Leave();
			SetEvent(m_hREvent);
		}
		else
			Leave();
	}

}

void BGRWLock::ReadUnlock()
{
	DecPendingLock();

	int nCount = InterlockedDecrement(&m_nCount);
	//if ( ( nCount & 0x8080 ) != 0 )
	//	LOG_ERR( L"CRWLock::ReadUnlock() count error" );
	if ((short)nCount == 0 && (nCount & 0xff000000))
	{
		SetEvent(m_hREvent);
	}
}

void BGRWLock::WriteLock()
{
	IncPendingLock();

	int nCount{ 0 };
	nCount = InterlockedExchangeAdd(&m_nCount, 0x100);
	if ((short)nCount)
	{
		Enter();
		do
		{
			nCount = InterlockedExchangeAdd(&m_nCount, 0x1000000 - 0x100) + 0x1000000 - 0x100;
			if ((short)nCount == 0)
			{
				Leave();
				SetEvent(m_hWEvent);
			}
			else
				Leave();

			WaitForSingleObject(m_hWEvent, INFINITE);
			Enter();
			nCount = InterlockedExchangeAdd(&m_nCount, -0x1000000 + 0x100);
		}
		while ((short)nCount);
		Leave();
	}
}

void BGRWLock::WriteUnlock()
{
	DecPendingLock();


	int	nCount = InterlockedExchangeAdd(&m_nCount, -0x100) - 0x100;
	//if ( ( nCount & 0x8080 ) != 0 )
	//	LOG_ERR( L"CRWLock::WriteUnlock() count error" );
	if ((short)nCount == 0 && (nCount & 0xff000000))
	{
		SetEvent(m_hWEvent);
	}
}

BOOL BGRWLock::WriteTryLock()
{
	if ((short)InterlockedExchangeAdd(&m_nCount, 0x100))
	{
		InterlockedExchangeAdd(&m_nCount, -0x100);
		return FALSE;
	}

	return TRUE;
}
//----------------------------------------------------------------------



//------------------------------- BGScopedLock ==================================//
BGScopedLock::BGScopedLock(BGILock& lock)
	: m_Lock(lock)
	, m_bLeaved(FALSE)
{
	m_Lock.Enter();
}

BGScopedLock::~BGScopedLock()
{
	if (m_bLeaved == FALSE)
		m_Lock.Leave();
}

void BGScopedLock::ForceLeave()
{
	if (m_bLeaved == TRUE)
		return;

	if (m_bLeaved == FALSE)
	{
		m_Lock.Leave();
		m_bLeaved = TRUE;
	}
}
//----------------------------------------------------------------------




//------------------------------- BGRWLock ==================================//
//----------------------------------------------------------------------