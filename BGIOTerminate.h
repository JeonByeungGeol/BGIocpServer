#pragma once

#include "BGIOObject.h"

class BGIOCompletionHandler;

class BGIOTerminate : public BGIOObject
{
public:
	BGIOTerminate() = default;
	~BGIOTerminate() = default;

			void	PostObject(BGIOCompletionHandler* pIOCPHandler, std::thread* pThread, HANDLE hEvent);
	virtual void	OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);

public:
	std::thread*		m_pThread;
	HANDLE				m_hEvent;
};

