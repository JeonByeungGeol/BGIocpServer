#include "stdafx.h"
#include "BGIOSocket.h"
#include "BGIOBuffer.h"
#include "BGIOCompletionHandler.h"

BGIOSocket::BGIOSocket(SOCKET s)
{
	m_hSocket = s;
	if (m_hSocket != INVALID_SOCKET)
		AddRef();

	memset(&m_overlappedRead, 0, sizeof(OVERLAPPED));
	memset(&m_overlappedWrite, 0, sizeof(OVERLAPPED));
	m_pReadBuf = BGIOBuffer::Alloc();
	m_nPendingWrite = 0;
	m_pFirstBuf = m_pLastBuf = nullptr;
}


BGIOSocket::~BGIOSocket()
{
	m_pReadBuf->Release();
	while (m_pFirstBuf)
	{
		BGIOBuffer* pBuf = m_pFirstBuf;
		m_pFirstBuf = m_pFirstBuf->m_pNext;
		pBuf->Free();
	}
}

void BGIOSocket::Initialize(BGIOCompletionHandler* pIOCPHandler)
{
	if (pIOCPHandler == nullptr)
		BG_LOG_ERROR("IOCPHandler is null");


	if (!pIOCPHandler->HandleAdd((HANDLE)m_hSocket, (ULONG_PTR)this)) {
		BG_LOG_ERROR("IOCPCompletionHandler->HandleAdd is Fail");
		Close();
		return;
	}

	int zero{ 0 };
	setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&zero, sizeof(zero));
	zero = 0;
	setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&zero, sizeof(zero));

	OnCreate();
}

void BGIOSocket::OnCreate()
{
	Read(0);
}

void BGIOSocket::Close()
{
	SOCKET hSocket = InterlockedExchange((LONG*)&m_hSocket, INVALID_SOCKET);
	if (hSocket != INVALID_SOCKET) {
		OnClose();
		LINGER linger;
		linger.l_onoff = 1;
		linger.l_linger = 0;
		setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));
		closesocket(hSocket);
		Release();
	}
}

void BGIOSocket::GracefulClose()
{
	SOCKET hSocket = InterlockedExchange((LONG*)&m_hSocket, INVALID_SOCKET);
	if (hSocket != INVALID_SOCKET) {
		OnClose();
		shutdown(hSocket, SD_BOTH);
		LINGER linger;
		linger.l_onoff = 1;
		linger.l_linger = 5;
		setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));
		closesocket(hSocket);
		Release();
	}
}

void BGIOSocket::Read(DWORD dwLeft)
{
	m_pReadBuf->m_dwSize -= dwLeft;
	if (m_pReadBuf->m_nRef != 1)
	{
		BGIOBuffer* pNextBuf = BGIOBuffer::Alloc();
		memcpy(pNextBuf->m_buffer, m_pReadBuf->m_buffer + m_pReadBuf->m_dwSize, dwLeft);
		m_pReadBuf->Release();
		m_pReadBuf = pNextBuf;
	}
	else
	{
		memmove(m_pReadBuf->m_buffer, m_pReadBuf->m_buffer + m_pReadBuf->m_dwSize, dwLeft);
	}
	m_pReadBuf->m_dwSize = dwLeft;

	AddRef();
	WSABUF wsabuf;
	wsabuf.len = BUFFER_SIZE - m_pReadBuf->m_dwSize;
	wsabuf.buf = m_pReadBuf->m_buffer + m_pReadBuf->m_dwSize;
	DWORD dwRecv;
	DWORD dwFlag = 0;
	if (WSARecv(m_hSocket, &wsabuf, 1, &dwRecv, &dwFlag, &m_overlappedRead, NULL)
		&& GetLastError() != ERROR_IO_PENDING)
	{
		int nErr = GetLastError();
		if (nErr != WSAENOTSOCK && nErr != WSAECONNRESET && nErr != WSAECONNABORTED)
			BG_LOG_ERROR("");
		Close();

		Release();
	}
}

void BGIOSocket::Write(BGIOBuffer* pBuffer)
{
	if (pBuffer->m_dwSize == 0)
	{
		pBuffer->Free();
		return;
	}
	if (pBuffer->m_dwSize > BUFFER_SIZE)
	{
		BG_LOG_ERROR("buffer size over");
		pBuffer->Free();
		return;
	}
	m_lock.Enter();
	m_nPendingWrite += pBuffer->m_dwSize;
	if (m_pLastBuf == nullptr)
	{
		m_pFirstBuf = m_pLastBuf = pBuffer;
		m_lock.Leave();
		AddRef();
		WSABUF wsabuf;
		wsabuf.len = pBuffer->m_dwSize;
		wsabuf.buf = pBuffer->m_buffer;
		DWORD dwSent;
		if (WSASend(m_hSocket, &wsabuf, 1, &dwSent, 0, &m_overlappedWrite, nullptr)
			&& GetLastError() != ERROR_IO_PENDING)
		{
			int nErr = GetLastError();
			if (nErr != WSAENOTSOCK && nErr != WSAECONNRESET && nErr != WSAECONNABORTED)
				BG_LOG_ERROR("");
			Release();
		}
	}
	else if (m_pFirstBuf != m_pLastBuf && m_pLastBuf->m_dwSize + pBuffer->m_dwSize <= BUFFER_SIZE)
	{
		memcpy(m_pLastBuf->m_buffer + m_pLastBuf->m_dwSize, pBuffer->m_buffer, pBuffer->m_dwSize);
		m_pLastBuf->m_dwSize += pBuffer->m_dwSize;
		m_lock.Leave();
		pBuffer->Free();
	}
	else
	{
		m_pLastBuf->m_pNext = pBuffer;
		m_pLastBuf = pBuffer;
		m_lock.Leave();
	}

}

long BGIOSocket::PendingWrite() { return m_nPendingWrite; }

void BGIOSocket::OnIOCallback(BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped)
{
	if (!bSuccess)
	{
		if (lpOverlapped == &m_overlappedRead)
			Close();
		else
			BG_LOG_ERROR("this");
	}
	else if (lpOverlapped == &m_overlappedWrite) {
		WriteCallback(dwTransferred);
		Release();
	}
	else if (lpOverlapped == &m_overlappedRead) {
		ReadCallback(dwTransferred);
		Release();
	}
	else
	{
		if (lpOverlapped)
			BG_LOG_ERROR("lpOverlapped error");
		OnTimer(dwTransferred);
		Release(&m_nTimerRef);
	}


}

void BGIOSocket::ReadCallback(DWORD dwTransferred)
{
	if (dwTransferred == 0)
	{
		Close();
		return;
	}
	m_pReadBuf->m_dwSize += dwTransferred;
	OnRead();
}

void BGIOSocket::WriteCallback(DWORD dwTransferred)
{
	m_lock.Enter();
	if (dwTransferred != m_pFirstBuf->m_dwSize)
	{
		BG_LOG_ERROR("");
		m_lock.Leave();
		return;
	}
	m_nPendingWrite -= m_pFirstBuf->m_dwSize;
	BGIOBuffer* pFirstBuf = m_pFirstBuf;
	if ((m_pFirstBuf = m_pFirstBuf->m_pNext) != nullptr)
	{
		m_lock.Leave();
		AddRef();
		WSABUF wsabuf;
		wsabuf.len = m_pFirstBuf->m_dwSize;
		wsabuf.buf = m_pFirstBuf->m_buffer;
		DWORD dwSent;
		if (WSASend(m_hSocket, &wsabuf, 1, &dwSent, 0, &m_overlappedWrite, nullptr)
			&& GetLastError() != ERROR_IO_PENDING)
		{
			int nErr = GetLastError();
			if (nErr != WSAENOTSOCK && nErr != WSAECONNRESET && nErr != WSAECONNABORTED)
				BG_LOG_ERROR("%llx(%p), err=%d", m_hSocket, this, nErr);
			Release();
		}
	}
	else
	{
		m_pLastBuf = nullptr;
		m_lock.Leave();
	}
	pFirstBuf->Free();
}