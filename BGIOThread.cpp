#include "stdafx.h"
#include "BGIOThread.h"

BGIOThread::THREADLIST BGIOThread::s_threadList;

/** static*/
void BGIOThread::Terminate()
{
	for (auto &th : s_threadList) {
		th->OnTerminate();
		delete th;
	}

	s_threadList.clear();
}

BGIOThread::BGIOThread()
	:m_bStarted(FALSE), m_pThread(nullptr), m_pIOCPHandler(nullptr)
{

}

BGIOThread::~BGIOThread()
{
}

/**
 * 이곳에서 스레드 생성 및 실행 후에 s_threadList에 등록한다.
*/
void BGIOThread::Begin(BGIOCompletionHandler * pIOCPHandler)
{
	if (m_bStarted == TRUE) {
		BG_LOG_WARNING("already running");
		return;
	}
	
	m_pIOCPHandler = pIOCPHandler;

	m_pThread = spawn();

	s_threadList.push_back(this);

	m_bStarted = TRUE;

	return;
}

std::thread* BGIOThread::spawn()
{
	return new std::thread{ [this] {this->Run(); } };
}