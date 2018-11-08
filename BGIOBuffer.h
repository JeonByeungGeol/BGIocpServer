#pragma once

#include "BGLock.h"

class BGIOBuffer
{
public:
	class BGSlot
	{
	public:
			BGIOBuffer*		m_pBuffer;
			BGIOSpinLock		m_lock;

			BGSlot() : m_pBuffer(nullptr) {};
	};

public:
	static	BGIOBuffer*		Alloc();
	static	void			FreeAll();

public:
			void			Free();
			void			AddRef() { InterlockedIncrement(&m_nRef); }
			void			Release() { if (InterlockedDecrement(&m_nRef) == 0) Free(); }

	template<typename PacketT>
	void AssignSize(char* end) {
		PacketT* packet				= (PacketT*)m_buffer;
		m_dwSize = packet->wSize	= end - (char*)packet;
	}


private:
	/** IO��ü ���� Ref*/
	LONG					m_nRef;

	/** ����*/
	char					m_buffer[BG_BUFFER_SIZE];

	/** ���� ������*/
	DWORD					m_dwSize;

	/** ��ü�� ��Ȱ���ϱ� ���� ���ο����� ����ϴ� ���*/
	BGIOBuffer*				m_pNext;
};

