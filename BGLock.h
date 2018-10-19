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

