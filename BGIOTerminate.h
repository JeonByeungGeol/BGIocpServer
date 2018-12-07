#pragma once

#include "BGIOObject.h"

class BGIOCompletionHandler;

class BGIOTerminate : public BGIOObject
{
public:
	BGIOTerminate() = default;
	~BGIOTerminate() = default;

			void	PostObject(BGIOCompletionHandler* pIOCPHandler, std::thread* pThread);
	virtual void	OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);

public:
	std::thread*		m_pThread;
};

