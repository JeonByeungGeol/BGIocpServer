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
	virtual ~BGILock();

	virtual void Enter() {};
	virtual void Leave() {};

	void IncPendingLock();
	void DecPendingLock();
};





class BGRWLock : public BGILock
{
public:
	virtual void Enter() {}
	virtual void Leave() {}

	void WriteLock() {};
	void WriteUnlock() {};

	void ReadLock() {};
	void ReadUnlock() {};
};

