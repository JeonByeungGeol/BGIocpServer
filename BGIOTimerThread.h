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




	/** static 멤버*/
public:
	/** 해당 함수를 호출해야 타이머 스레드가 시작됩니다.*/
	static	void					Init();

	/** 타이머 객체를 추가합니다.*/
	static	void					AddTimer(BGIOObject* pObject, ULONGLONG n64Time, int nId);

	/** 타이머 객체가 저장된 큐*/
	static	TimerQueue				s_timerQueue;

	/** 타이머 관리에 쓰일 동기화 객체*/
	static	BGIOCriticalSection		s_lockTimer;

	/** 타이머 큐 가장 앞에 있는 객체의 topTime*/
	static	ULONGLONG				s_n64TopTime;

	/** 타이머에 새로운 객체가 들어왔는지 확인하는 Event객체*/
	static	HANDLE					s_hTimer;

	/** 타이머 스레드가 동작중인지 여부*/
	static	BOOL					s_nStop;

	/** 타이머가 비어있지 않도록 하기 위한 객체*/
	static	BGIOTimerInstance		s_TimerInstance;
	
public:
	BGIOTimerThread();
	virtual ~BGIOTimerThread();

private:
	virtual	void					Run();
	virtual	void					OnTerminate();


protected:
	/** Index 0 : 타이머객체가 들어왔는지 확인하는 이벤트, Index 1 : 타이머 기능 종료를 알리는 이벤트*/
	HANDLE							m_vHandle[2];
};

