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

/**
  * IOSpin Lock 객체
  *
  */
class BGIOSpinLock : public BGILock
{
public:
	BGIOSpinLock();

	virtual void Enter();
	virtual void Leave();
	BOOL TryEnter();

protected:
	void Wait();

	/** lock 여부 (1: lock, 0: unlock)*/
	long m_nLock;
};

/**
  * IOCriticalSection 객체
  *
  */
class BGIOCriticalSection : public BGILock
{
public:
	CRITICAL_SECTION m_critical_section;

public:
	BGIOCriticalSection() { InitializeCriticalSection(&m_critical_section); }
	BGIOCriticalSection(DWORD dwSpinCount) { (void)InitializeCriticalSectionAndSpinCount(&m_critical_section, dwSpinCount); }
	~BGIOCriticalSection() { DeleteCriticalSection(&m_critical_section); }

	_Acquires_lock_(m_critical_section)
		void	Enter() { IncPendingLock(); EnterCriticalSection(&m_critical_section); }

	_Releases_lock_(m_critical_section)
		void	Leave() { DecPendingLock(); LeaveCriticalSection(&m_critical_section); }

	BOOL	TryEnter() { return TryEnterCriticalSection(&m_critical_section); }
};


/**
  * RW Lock 객체
  * Read, Wtite 동기화 수준을 다르게 할 수 있다.
  */
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
	/**  */
	long m_nCount;

	/** 1 또는 0 유지 */
	long m_nLock;
	HANDLE m_hREvent;
	HANDLE m_hWEvent;
	
	void Wait();
	virtual void Enter();
	virtual void Leave();
};


/**
  * Scoped Lock 객체
  
  */
struct BGScopedLock
{
public:
	explicit BGScopedLock(BGILock& lock);
	~BGScopedLock();

	void ForceLeave();
	
private:
	/** m_Lock가 locking중인지 여부*/
	BOOL m_bLeaved;

	/** Scoped에서 사용할 lock 객체*/
	BGILock& m_Lock;
};