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

	/**  IOCP��ü�� ���, ���� listen, Accept �����带 �����մϴ�.*/
			bool			Start(BGIOCompletionHandler* pIOCPHandler, int nPort);

	/** ������ �ݰ�, Accept �����带 �����մϴ�.*/
			void			Close();

	/** ������ �ݰ� nulló�� �մϴ�.*/
			void			Stop();

protected:
	/** AcceptThread���� �̺�Ʈ�� �߻����� �� ȣ��*/
	virtual	void			OnWaitCallback();

	/** IOCP Handler�� ���� ȣ��Ǵ� �Լ�*/
	virtual	void			OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);

	/** Ŭ���̾�Ʈ ���� �� ó��*/
	virtual	BGIOSocket*		CreateSocket(SOCKET s, sockaddr_in* addr) = 0;


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

