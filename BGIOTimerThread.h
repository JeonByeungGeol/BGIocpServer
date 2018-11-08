#pragma once

#include "BGIOThread.h"
#include "BGIOTimer.h"
#include "BGIOObject.h"
#include "BGLock.h"
class BGIOTimerThread : public BGIOThread
{
	using TimerVector	= std::vector<BGIOTimer>;
	using TimerQueue	= std::priority_queue<BGIOTimer, TimerVector>;

public:
	class BGIOTimerInstance : public BGIOObject {
	public:
		virtual void			OnTimerCallback(int id = 0);
	};




	/** static ���*/
public:
	/** �ش� �Լ��� ȣ���ؾ� Ÿ�̸� �����尡 ���۵˴ϴ�.*/
	static	void					Init();

	/** Ÿ�̸� ��ü�� �߰��մϴ�.*/
	static	void					AddTimer(BGIOObject* pObject, ULONGLONG n64Time, int nId);

	/** Ÿ�̸� ��ü�� ����� ť*/
	static	TimerQueue				s_timerQueue;

	/** Ÿ�̸� ������ ���� ����ȭ ��ü*/
	static	BGIOCriticalSection		s_lockTimer;

	/** Ÿ�̸� ť ���� �տ� �ִ� ��ü�� topTime*/
	static	ULONGLONG				s_n64TopTime;

	/** Ÿ�̸ӿ� ���ο� ��ü�� ���Դ��� Ȯ���ϴ� Event��ü*/
	static	HANDLE					s_hTimer;

	/** Ÿ�̸� �����尡 ���������� ����*/
	static	BOOL					s_nStop;

	/** Ÿ�̸Ӱ� ������� �ʵ��� �ϱ� ���� ��ü*/
	static	BGIOTimerInstance		s_TimerInstance;
	
public:
	BGIOTimerThread();
	virtual ~BGIOTimerThread();

private:
	virtual	void					Run();
	virtual	void					OnTerminate();


protected:
	/** Index 0 : Ÿ�̸Ӱ�ü�� ���Դ��� Ȯ���ϴ� �̺�Ʈ, Index 1 : Ÿ�̸� ��� ���Ḧ �˸��� �̺�Ʈ*/
	HANDLE							m_vHandle[2];
};

