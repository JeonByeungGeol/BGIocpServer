#pragma once

#include "BGIOThread.h"
#include "BGIOTimer.h"
#include "BGIOObject.h"

class BGIOTimerThread : public BGIOThread
{
	using TimerVector = std::vector<BGIOTimer>;
	using TimerQueue = std::priority_queue<BGIOTimer, TimerVector>;

public:
	class BGIOTimerInstance : public BGIOObject {
	public:
		virtual void OnTimerCallback(int id = 0);
	};




	/** static*/
public:
	static void Init();
	static void AddTimer(BGIOObject* pObject, ULONGLONG n64Time, int nId);


	static TimerQueue s_timerQueue;
	// static BGIOCriticalSection s_lockTimer;
	static ULONGLONG s_n64TopTime;
	static HANDLE s_hTimer;
	static BGIOTimerInstance s_TimerInstance;
	static BOOL s_nStop;
	
public:
	BGIOTimerThread();
	virtual ~BGIOTimerThread();

private:
	virtual void Run();
	virtual void OnTerminate();


protected:
	HANDLE m_vHandle[2];
	BGIOObject* m_pObject;
};

