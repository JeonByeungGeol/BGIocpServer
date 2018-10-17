#pragma once
class BGIOBuffer
{
public:
	static BGIOBuffer* Alloc();
	static void FreeAll();

public:
	void Free();
	void AddRef() { InterlockedIncrement(&m_nRef); }
	void Release() { if (InterlockedDecrement(&m_nRef) == 0) Free(); }

	template<typename PacketT>
	void AssignSize(char* end) {

	}


private:
	/** IO°´Ã¼ °ü¸® Ref*/
	LONG m_nRef;

	/** */
	char m_buffer[BG_BUFFER_SIZE];
	DWORD m_dwSize;
	BGIOBuffer* m_pNext;


public:
	class BGSlot {
	public:
		BGIOBuffer* m_pBuffer;
		std::mutex m_lock;

		BGSlot() : m_pBuffer(nullptr) {};
	};
};

