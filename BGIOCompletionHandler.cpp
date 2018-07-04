#include "stdafx.h"
#include "BGIOCompletionHandler.h"


BGIOCompletionHandler::BGIOCompletionHandler()
	: m_hCompletionPort (INVALID_HANDLE_VALUE)
{
}


BGIOCompletionHandler::~BGIOCompletionHandler()
{
	CloseHandle(m_hCompletionPort);
	m_hCompletionPort = INVALID_HANDLE_VALUE;
}

BOOL BGIOCompletionHandler::Init()
{
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (m_hCompletionPort == INVALID_HANDLE_VALUE)
		return FALSE;
	return TRUE;
}

BOOL BGIOCompletionHandler::HandleAdd(HANDLE handle, ULONG_PTR key)
{
	if (CreateIoCompletionPort(handle, m_hCompletionPort, key, 0) == nullptr)
		return FALSE;
	return TRUE;
}

BOOL BGIOCompletionHandler::Get(DWORD & dwTransferred, BGIOObject ** ppObject, OVERLAPPED ** ppOverlapped)
{
	return GetQueuedCompletionStatus(m_hCompletionPort, &dwTransferred, (PULONG_PTR)&(*ppObject), &(*ppOverlapped), INFINITE);
}

BOOL BGIOCompletionHandler::Post(int nId, BGIOObject * pObject)
{
	return PostQueuedCompletionStatus(m_hCompletionPort, nId, (ULONG_PTR)pObject, nullptr);
}
