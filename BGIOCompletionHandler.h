#pragma once

class BGIOObject;

class BGIOCompletionHandler
{
public:
	BGIOCompletionHandler();
	~BGIOCompletionHandler();

	/** IOCP ��ü�� �����մϴ�.*/
	BOOL Init();

	/** IOCP ��ü�� �����մϴ�.*/
	BOOL HandleAdd(HANDLE handle, ULONG_PTR key);
	
	/** GetQueuedCompletionStatus*/
	BOOL Get(DWORD& dwTransferred, BGIOObject** ppObject, OVERLAPPED** ppOverlapped);

	/** PostQueuedCompletionStatus*/
	BOOL Post(int nId, BGIOObject* pObject);


private:
	/** IOCP ��ü �ڵ� �Դϴ�.*/
	HANDLE m_hCompletionPort;
};

