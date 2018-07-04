#pragma once

#include "BGIOObject.h"

class BGIOSocket;
class BGIOAcceptThread;
class BGIOCompletionHandler;

class BGIOServer : public BGIOObject
{
public:
	BGIOServer();
	virtual ~BGIOServer();

	bool Start(BGIOCompletionHandler* pIOCPHandler, int nPort);
	void Close();
	void Stop();

protected:
	/***/
	virtual void OnWaitCallback();
	virtual void OnIOCallback();
	virtual BGIOSocket* CreateSocket(SOCKET s, sockaddr_in* addr) = 0;


private:
	/** 리스닝 소켓*/
	SOCKET m_hSocket;

protected:
	/** Accept 알림 이벤트*/
	HANDLE m_hAcceptEvent;

	/** Accept 기능을 하는 스레드*/
	BGIOAcceptThread* m_pAcceptThread;

	/** IOCP Handler*/
	BGIOCompletionHandler* m_pIOCPHandler;
};

