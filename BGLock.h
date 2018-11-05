#pragma once

class BGILock;
class BGRWLock;

/** 
  * ������ ���� ������ ����ȴ�.
  * �����帶�� �����ϴ� ��ü�� ������ �Ҹ�ÿ� �����ȴ�.
  */
class BGLockInfo
{
public:
	BGLockInfo();
	~BGLockInfo();

	/** Pending �� �� ���� */
	volatile DWORD m_dwPendingLock;

	/** Lock �� �ð� */
	volatile ULONGLONG m_n64LastLockedTicks;
};

/**
  * ���� ������ BGILock ��ü
  * static ������ LockInfo��ü�� map���� �����Ѵ�.
  * �ش� map�� ����ȭ ��Ű�� ���� rwLock�� ������ �ִ�.
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
  * IOSpin Lock ��ü
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

	/** lock ���� (1: lock, 0: unlock)*/
	long m_nLock;
};

/**
  * IOCriticalSection ��ü
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
  * RW Lock ��ü
  * Read, Wtite ����ȭ ������ �ٸ��� �� �� �ִ�.
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

	/** 1 �Ǵ� 0 ���� */
	long m_nLock;
	HANDLE m_hREvent;
	HANDLE m_hWEvent;
	
	void Wait();
	virtual void Enter();
	virtual void Leave();
};


/**
  * Scoped Lock ��ü
  
  */
struct BGScopedLock
{
public:
	explicit BGScopedLock(BGILock& lock);
	~BGScopedLock();

	void ForceLeave();
	
private:
	/** m_Lock�� locking������ ����*/
	BOOL m_bLeaved;

	/** Scoped���� ����� lock ��ü*/
	BGILock& m_Lock;
};