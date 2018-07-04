#pragma once

class BGIOObject;

class BGIOCompletionHandler
{
public:
	BGIOCompletionHandler();
	~BGIOCompletionHandler();

	/** IOCP 객체를 생성합니다.*/
	BOOL Init();

	/** IOCP 객체에 연결합니다.*/
	BOOL HandleAdd(HANDLE handle, ULONG_PTR key);
	
	/** GetQueuedCompletionStatus*/
	BOOL Get(DWORD& dwTransferred, BGIOObject** ppObject, OVERLAPPED** ppOverlapped);

	/** PostQueuedCompletionStatus*/
	BOOL Post(int nId, BGIOObject* pObject);


private:
	/** IOCP 객체 핸들 입니다.*/
	HANDLE m_hCompletionPort;
};

