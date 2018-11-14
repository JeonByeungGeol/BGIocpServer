#pragma once

#include "BGIOObject.h"
#include "BGIOBuffer.h"
#include "BGLock.h"

class BGIOCompletionHandler;

class BGIOSocket : public BGIOObject
{
public:
	BGIOSocket(SOCKET s);
	virtual ~BGIOSocket();

	/** 소켓 IOCPHandler에 등록후 OnCreate() 호출*/
	void				Initialize(BGIOCompletionHandler* pIOCPHandler);

	/** 소켓 초기화후 작업 (Read(0) 호출)*/
	virtual	void		OnCreate();

	/** */
	void		Close();

	/** */
	void		GracefulClose();
	void		Read(DWORD dwLeft);
	void		Write(BGIOBuffer* pBuffer);
	long		PendingWrite();

protected:
	virtual	void		OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped);
	void				ReadCallback(DWORD dwTransferred);
	void				WriteCallback(DWORD dwTransferred);
	virtual	void		OnClose() = 0;
	virtual	void		OnRead() = 0;


public:
	SOCKET			m_hSocket;

protected:
	BGIOCriticalSection			m_lock;
	OVERLAPPED					m_overlappedRead;
	OVERLAPPED					m_overlappedWrite;
	BGIOBuffer*					m_pReadBuf;
	BGIOBuffer*					m_pFirstBuf;
	BGIOBuffer*					m_pLastBuf;
	long						m_nPendingWrite;
};

