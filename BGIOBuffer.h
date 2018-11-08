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
	/** IO객체 관리 Ref*/
	LONG					m_nRef;

	/** 버퍼*/
	char					m_buffer[BG_BUFFER_SIZE];

	/** 버퍼 사이즈*/
	DWORD					m_dwSize;

	/** 객체를 재활용하기 위해 내부에서만 사용하는 멤버*/
	BGIOBuffer*				m_pNext;
};

