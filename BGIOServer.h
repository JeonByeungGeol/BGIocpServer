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
	/** ������ ����*/
	SOCKET m_hSocket;

protected:
	/** Accept �˸� �̺�Ʈ*/
	HANDLE m_hAcceptEvent;

	/** Accept ����� �ϴ� ������*/
	BGIOAcceptThread* m_pAcceptThread;

	/** IOCP Handler*/
	BGIOCompletionHandler* m_pIOCPHandler;
};

