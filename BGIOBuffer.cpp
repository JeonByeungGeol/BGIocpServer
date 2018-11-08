#include "stdafx.h"
#include "BGIOBuffer.h"

#define BUFFER_POOL_SIZE 2

static BGIOBuffer::BGSlot g_slotBuffer[BUFFER_POOL_SIZE];
static long g_nAllocBuffer = -1;
static long g_nFreeBuffer = 0;
static long g_nStop;

BGIOBuffer * BGIOBuffer::Alloc()
{
	BGSlot* pSlot = &g_slotBuffer[InterlockedIncrement(&g_nAllocBuffer) & (BUFFER_POOL_SIZE - 1)];
	BGIOBuffer* newBuffer{ nullptr };
	pSlot->m_lock.Enter();
	if ((newBuffer = pSlot->m_pBuffer) != nullptr) {
		pSlot->m_pBuffer = newBuffer->m_pNext;
		pSlot->m_lock.Leave();
	}
	else {
		pSlot->m_lock.Leave();
		newBuffer = new BGIOBuffer;
	}
	
	newBuffer->m_dwSize = 0;
	newBuffer->m_nRef = 1;
	newBuffer->m_pNext = nullptr;

	return newBuffer;
}

void BGIOBuffer::FreeAll()
{
	InterlockedExchange(&g_nStop, 1);

	for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
		BGSlot* pSlot = &g_slotBuffer[i];
		pSlot->m_lock.Enter();
		BGIOBuffer* pBuffer{ nullptr };
		while ((pBuffer = pSlot->m_pBuffer) != nullptr) {
			pSlot->m_pBuffer = pBuffer->m_pNext;
			delete pBuffer;
		}
		pSlot->m_lock.Leave();
	}
}

void BGIOBuffer::Free()
{
	if (g_nStop) {
		delete this;
		return;
	}
	
	BGSlot* pSlot = &g_slotBuffer[InterlockedDecrement(&g_nFreeBuffer) & (BUFFER_POOL_SIZE - 1)];
	pSlot->m_lock.Enter();
	m_pNext = pSlot->m_pBuffer;
	pSlot->m_pBuffer = this;
	pSlot->m_lock.Leave();
}
