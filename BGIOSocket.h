#pragma once

#include "BGIOObject.h"
#include "BGIOBuffer.h"
#include "BGLock.h"

class BGIOCompletionHandler;

/**
 * [BGIOSocket 객체]
 * 
 * 초기화
 * Initialize()		->	OnRead()
 *
 * IO동작
 * OnIOCallback()	->	ReadCallback()		-> OnRead()		->	Process() 
 *					->	WriteCallback()		-> OnWrite()
 *					->	OnTimer()
 *
 * 
 * BGIOSocket 객체를 상속후에 OnRead()를 구현
 * OnRead에서 패킷 처리할 Process를 가진 객체를 만든 후에
 * IOCP객체를 통해 호출해 준다.
 */

class BGIOSocket : public BGIOObject
{
public:
	BGIOSocket(SOCKET s);
	virtual ~BGIOSocket();

	/** 소켓 IOCPHandler에 등록후 OnCreate() 호출*/
			void				Initialize(BGIOCompletionHandler* pIOCPHandler);

	/** 소켓 초기화후 작업 (Read(0) 호출)*/
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

	/** IOCP Handler를 통해 호출되는 함수*/
	virtual	void				OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);

	/** WSARecv 호출 이후 OnIOCallback에서 호출되는 함수*/
			void				ReadCallback(DWORD dwTransferred);

	/** WSASend 호출 이후 OnIOCallback에서 호출되는 함수*/
			void				WriteCallback(DWORD dwTransferred);

	/** ReadCallback 에서 호출*/
	virtual	void				OnRead() = 0;
	


public:
	SOCKET						m_hSocket;

protected:
	BGIOCriticalSection			m_lock;

	/** WSARecv에서 사용하는 버퍼*/
	OVERLAPPED					m_overlappedRead;

	/** WSASend에서 사용하는 버퍼*/
	OVERLAPPED					m_overlappedWrite;

	/***/
	BGIOBuffer*					m_pReadBuf;
	BGIOBuffer*					m_pFirstBuf;
	BGIOBuffer*					m_pLastBuf;
	long						m_nPendingWrite;
};

