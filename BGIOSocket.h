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

	/** 
	 * AcceptThread���� �ſ��� ���� IOServer::IOCallback�� ȣ�� �ǰ�
	 * IOCallback���� ȣ��ȴ�.
	 * ���� IOCPHandler�� ����� OnCreate() ȣ��
	 */
			void				Initialize(BGIOCompletionHandler* pIOCPHandler);

	/** 
	 * Initialize �ȿ��� ȣ�� �ȴ�.
	 * ���� �ʱ�ȭ�� �۾� (Read(0) ȣ��)	 
	 */
	virtual	void				OnCreate();

	/** io_size�� ũ�Ⱑ 0�϶� ���� ����� �Ǵ��ϰ� ȣ��*/
			void				Close();

	/** */
			void				GracefulClose();
			void				Read(DWORD dwLeft);
			void				Write(BGIOBuffer* pBuffer);
			long				PendingWrite();

protected:
	/** io_size�� ũ�Ⱑ 0�϶�, Close()�� ȣ���ϰ� �װ����� OnClose() ȣ��*/
	virtual	void				OnClose() = 0;

	/** IOCP Handler�� ���� ȣ��Ǵ� �Լ�*/
	virtual	void				OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);

	/**
	  * WSARecv ȣ�� ���� OnIOCallback���� ȣ��Ǵ� �Լ�
	  * m_pReadBuf�� dwSize�� ������Ű��, OnRead�� ȣ���Ѵ�.
	  */
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

	/** ������ ������ ����*/
	BGIOBuffer*					m_pFirstBuf;

	/** ������ ������ ����*/
	BGIOBuffer*					m_pLastBuf;

	/** ���� �������� ������ ũ��*/
	long						m_nPendingWrite;
};

