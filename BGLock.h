#pragma once

class BGILock;
class BGRWLock;

/** 
 * 스레드 로컬 변수로 선언된다.
 * 스레드마다 관리하는 객체로 스레드 소멸시에 삭제된다.
*/
class BGLockInfo
{
public:
	BGLockInfo();
	~BGLockInfo();

	/** Pending 된 락 갯수 */
	volatile DWORD m_dwPendingLock;

	/** Lock 된 시각 */
	volatile ULONGLONG m_n64LastLockedTicks;
};

/**
 * 가장 상위의 BGILock 객체
 * static 변수로 LockInfo객체를 map으로 관리한다.
 * 해당 map을 동기화 시키기 위한 rwLock을 가지고 있다.
*/
class BGILock
{
public:
	static BGRWLock s_rwLock;

	static std::map<BGLockInfo*, BGLockInfo*>& GetLockInfoMap();
	static BOOL IsDeadLock(int nLimitSec);

public:
	BGILock();

	virtual void Enter() = 0;
	virtual void Leave() = 0;

	void IncPendingLock();
	void DecPendingLock();
};

class BGRWLock : public BGILock
{
public:
	BGRWLock();
	~BGRWLock();

	void ReadLock();
	void ReadUnlock();
	void WriteLock();
	void WriteUnlock();
	BOOL WriteTryLock();

protected:
	long m_nCount;
	long m_nLock;
	HANDLE m_hREvent;
	HANDLE m_hWEvent;
	
	void Wait();
	virtual void Enter();
	virtual void Leave();
};

