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

BGILock::~BGILock()
{
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
