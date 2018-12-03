#pragma once

#include "BGIOObject.h"
#include "BGIOBuffer.h"
#include "BGLock.h"

class BGIOCompletionHandler;

/**
 * [BGIOSocket ��ü]
 * 
 * �ʱ�ȭ
 * Initialize()		->	OnRead()
 *
 * IO����
 * OnIOCallback()	->	ReadCallback()		-> OnRead()		->	Process() 
 *					->	WriteCallback()		-> OnWrite()
 *					->	OnTimer()
 *
 * 
 * BGIOSocket ��ü�� ����Ŀ� OnRead()�� ����
 * OnRead���� ��Ŷ ó���� Process�� ���� ��ü�� ���� �Ŀ�
 * IOCP��ü�� ���� ȣ���� �ش�.
 */

class BGIOSocket : public BGIOObject
{
public:
	BGIOSocket(SOCKET s);
	virtual ~BGIOSocket();

	/** ���� IOCPHandler�� ����� OnCreate() ȣ��*/
			void				Initialize(BGIOCompletionHandler* pIOCPHandler);

	/** ���� �ʱ�ȭ�� �۾� (Read(0) ȣ��)*/
	virtual	void				OnCreate();

	/** */
			void				Close();

	/** */
			void				GracefulClose();
			void				Read(DWORD dwLeft);
			void				Write(BGIOBuffer* pBuffer);
			long				PendingWrite();

protected:
	/** */
	virtual	void				OnClose() = 0;

	/** IOCP Handler�� ���� ȣ��Ǵ� �Լ�*/
	virtual	void				OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);

	/** WSARecv ȣ�� ���� OnIOCallback���� ȣ��Ǵ� �Լ�*/
			void				ReadCallback(DWORD dwTransferred);

	/** WSASend ȣ�� ���� OnIOCallback���� ȣ��Ǵ� �Լ�*/
			void				WriteCallback(DWORD dwTransferred);

	/** ReadCallback ���� ȣ��*/
	virtual	void				OnRead() = 0;
	


public:
	SOCKET						m_hSocket;

protected:
	BGIOCriticalSection			m_lock;

	/** WSARecv���� ����ϴ� ����*/
	OVERLAPPED					m_overlappedRead;

	/** WSASend���� ����ϴ� ����*/
	OVERLAPPED					m_overlappedWrite;

	/***/
	BGIOBuffer*					m_pReadBuf;
	BGIOBuffer*					m_pFirstBuf;
	BGIOBuffer*					m_pLastBuf;
	long						m_nPendingWrite;
};

