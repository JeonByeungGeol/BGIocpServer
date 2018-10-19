#include "stdafx.h"
#include "BGLock.h"


//------------------------------- ABCDE ==================================//
//----------------------------------------------------------------------
thread_local BGLockInfo tlsLockInfo; //C++11 표준 TLS 변수
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
// static 멤버함수
std::map<BGLockInfo*, BGLockInfo*>& BGILock::GetLockInfoMap()
{
	static std::map<BGLockInfo*, BGLockInfo*> s_mapLockList;
	return s_mapLockList;
}

// static 멤버함수
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
	InterlockedExchange(&tlsLockInfo.m_n64LastLockedTicks, ::GetTickCount64()); // //먼저 해줘야 오랜만에 걸린 락에 대한 오류가 안생김
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
	// 멀티쓰레드 프로그래밍에서 락을 잡는 타이밍이 짧다면 이론족으로 spin lock을 사용하여 busy waiting을 하는 것이 성능에 좋다고 알려져 있다.
	// 하지만 실제로 spin lock으로 효과를 보기 쉽지 않은데, CPU에 이를 위해 pause라는 명령어가 존재한다.
	// spin lock이라는게 결국 변수의 값이 원하는 값이 될때까지 계속 loop를 돌리는 것인데,
	// puase라는 명령어를 통해 CPU에게 지금 spin loop를 돌고 있다는 힌트를 주고,
	// CPU 가 메모리버스 속도 등을 감안해서 변수가 바뀌는데 필요한 시간만큼 다음 명령어를 실행하지 않는 것이다.
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