#pragma once

#include "BGIOThread.h"

class BGIOObject;

class BGIOAcceptThread : public BGIOThread
{
public:
	BGIOAcceptThread(HANDLE hAcceptEvent, BGIOObject* pServer);
	virtual ~BGIOAcceptThread();

	/** 스레드 동작 함수입니다.*/
	virtual void Run();

	/** 스레드가 종료하기 직전에 호출되는 함수입니다.*/
	virtual void OnTerminate();

protected:
	/** 첫 번째는 종료이벤트, 두 번째는 selectevent모델에서 사용하는 핸들입니다.*/
	HANDLE m_vHandle[2];

	/** 부모 서버 객체입니다*/
	BGIOObject* m_pObject;
};

