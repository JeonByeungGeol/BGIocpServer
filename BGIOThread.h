#pragma once

class BGIOCompletionHandler;

class BGIOThread
{
	using THREADLIST = std::list<BGIOThread*>;

	/** static ���*/
public:
	/** ��ϵǾ� �ִ� ��� �����带 ���� ��, �����մϴ�.*/
	static	void			Terminate();

private:
	/** ��� �����带 �̰��� ����մϴ�.*/
	static	THREADLIST		s_threadList;
	

public:
	BGIOThread();
	virtual ~BGIOThread();

	/** �����带 �����Ͽ� Run �Լ��� �����մϴ�.*/
	void					Begin(BGIOCompletionHandler* pIOCPHandler);
	
	/** �����尡 ���� ������ ȣ��Ǵ� �Լ��Դϴ�.*/
	virtual	void			OnTerminate() {}
	
private:
	/** ���� ����*/
	BOOL					m_bStarted;

	/** ������ ������*/
	std::thread*			m_pThread;
	
	/** �����忡 ���ϵ� IOCP��ü �Դϴ�.*/
	BGIOCompletionHandler*	m_pIOCPHandler;

	/** ������ ���� �Լ� �Դϴ�.*/
	virtual void Run() {}

	/** �������Լ��� ����Լ��� �����ϱ� ���� �Լ��Դϴ�.*/
	std::thread* spawn();
};

