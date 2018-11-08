#pragma once

class BGIOCompletionHandler;

class BGIOThread
{
	using THREADLIST = std::list<BGIOThread*>;

	/** static 멤버*/
public:
	/** 등록되어 있는 모든 스레드를 종료 후, 삭제합니다.*/
	static	void			Terminate();

private:
	/** 모든 스레드를 이곳에 등록합니다.*/
	static	THREADLIST		s_threadList;
	

public:
	BGIOThread();
	virtual ~BGIOThread();

	/** 스레드를 생성하여 Run 함수를 실행합니다.*/
	void					Begin(BGIOCompletionHandler* pIOCPHandler);
	
	/** 스레드가 종료 직전에 호출되는 함수입니다.*/
	virtual	void			OnTerminate() {}
	
private:
	/** 동작 여부*/
	BOOL					m_bStarted;

	/** 스레드 포인터*/
	std::thread*			m_pThread;
	
	/** 스레드에 동록된 IOCP객체 입니다.*/
	BGIOCompletionHandler*	m_pIOCPHandler;

	/** 스레드 실행 함수 입니다.*/
	virtual void Run() {}

	/** 스레드함수를 멤버함수로 동작하기 위한 함수입니다.*/
	std::thread* spawn();
};

