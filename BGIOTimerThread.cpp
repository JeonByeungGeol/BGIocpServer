#include "stdafx.h"
#include "BGIOTimerThread.h"
#include "BGIOCompletionHandler.h"

BGIOTimerThread::TimerQueue			BGIOTimerThread::s_timerQueue;
BGIOCriticalSection					BGIOTimerThread::s_lockTimer;
ULONGLONG							BGIOTimerThread::s_n64TopTime;
HANDLE								BGIOTimerThread::s_hTimer;
BGIOTimerThread::BGIOTimerInstance	BGIOTimerThread::s_TimerInstance;
BOOL								BGIOTimerThread::s_nStop;

void BGIOTimerThread::BGIOTimerInstance::OnTimerCallback(int nId)
{
	// 타이머 큐가 비어있는 상태를 만들지 않기 위함
	s_timerQueue.push( BGIOTimer( (BGIOObject*)this, GetTickCount64() + 24 * 3600 * 1000, nId) );
	s_n64TopTime = s_timerQueue.top().m_n64Time;
}

BGIOTimerThread::BGIOTimerThread()
{
	s_nStop = TRUE;

	s_hTimer = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_vHandle[0] = s_hTimer;
	m_vHandle[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
}

BGIOTimerThread::~BGIOTimerThread()
{
	CloseHandle(m_vHandle[1]);
}

void BGIOTimerThread::Init()
{
	s_TimerInstance.OnTimerCallback();
	s_TimerInstance.AddRef();

	BGIOTimerThread* pTimerThread = new BGIOTimerThread();
	pTimerThread->Begin(nullptr);
}

void BGIOTimerThread::AddTimer(BGIOObject* pObject, ULONGLONG n64Time, int nId)
{
	n64Time += GetTickCount64();

	s_lockTimer.Enter();
	if (s_nStop) {
		BG_LOG_WARNING("timer stoped");
		s_lockTimer.Leave();
		return;
	}

	s_timerQueue.push( BGIOTimer(pObject, n64Time, nId) );

	if ((LONG)(s_n64TopTime > n64Time)) {
		s_n64TopTime = n64Time;
		s_lockTimer.Leave();
		SetEvent(s_hTimer);
	}
	else {
		s_lockTimer.Leave();
	}
}

void BGIOTimerThread::Run()
{
	s_nStop = FALSE;

	for (; ;) {
		ULONGLONG n64Tick = GetTickCount64();

		s_lockTimer.Enter();
		LONG nWait = (LONG)(s_n64TopTime - n64Tick);
		if (nWait <= 0) {
			const BGIOTimer &top = s_timerQueue.top();
			BGIOObject *pObject = top.m_pObject;
			int id = top.m_nId;
			for (; ;) {
				s_timerQueue.pop();
				pObject->OnTimerCallback(id);

				const BGIOTimer &nextTop = s_timerQueue.top();
				nWait = (LONG)(nextTop.m_n64Time - n64Tick);
				if (nWait > 0) {
					s_n64TopTime = nextTop.m_n64Time;
					break;
				}
				pObject = nextTop.m_pObject;
				id = nextTop.m_nId;
			}
		}
		s_lockTimer.Leave();

		
		


		DWORD dwWaitResult = WaitForMultipleObjects(2, m_vHandle, FALSE, nWait);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// 새로운 타이머 동작 들어옴
			continue;
		}
		else if (dwWaitResult == (WAIT_OBJECT_0 + 1)) {
			// 현재 시점 남은 타이머 동작 실행 후 종료
			s_lockTimer.Enter();
			s_nStop = TRUE;
			for (; ;) {
				if (s_timerQueue.empty())
					break;
				BGIOObject *pObject = s_timerQueue.top().m_pObject;
				pObject->Release(&pObject->m_nTimerRef);
				s_timerQueue.pop();
			}
			s_lockTimer.Leave();
			break;
		}
	}
}

void BGIOTimerThread::OnTerminate()
{
	SetEvent(m_vHandle[1]);
}