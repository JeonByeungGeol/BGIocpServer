#include "stdafx.h"
#include "BGLock.h"


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

	int count{ 0 };

	count = 4000;
	while (--count >= 0) {
		if(InterlockedExchange(&m_nLock, 1) == 0)
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

}

void BGRWLock::ReadUnlock()
{

}

void BGRWLock::WriteLock()
{

}

void BGRWLock::WriteUnlock()
{

}

BOOL BGRWLock::WriteTryLock()
{
	return TRUE;
}
//----------------------------------------------------------------------