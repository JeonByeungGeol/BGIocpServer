#pragma once

class CIOObject;

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
	BOOL Get(DWORD& dwTransferred, CIOObject** ppObject, OVERLAPPED** ppOverlapped);

	/** PostQueuedCompletionStatus*/
	BOOL Post(int nId, CIOObject* pObject);


private:
	/** IOCP ��ü �ڵ� �Դϴ�.*/
	HANDLE m_hCompletionPort;
};

