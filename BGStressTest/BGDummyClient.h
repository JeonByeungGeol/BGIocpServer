#pragma once

// test °ª
#define WORKER_THREAD_NUM 1
#define TEST_CLIENT_NUM 1



#define MAX_BUFF_SIZE 4096

#define CLIENT_BIT_CONNECT		0x00000001
#define CLIENT_BIT_LOGIN		0x00000002

#define IO_RECV				0
#define IO_SEND				1


#define SERVER_PORT			8800

#define g_Client	BGDummyClient::s_Clients

struct OVERLAPPED_EX {
	WSAOVERLAPPED	m_Overlapped;
	int				m_io_type;
};

struct SEND_STRUCT {
	OVERLAPPED_EX	m_OverExSend;
	WSABUF			m_SendWSABuf;
	char			m_SendBuffer[MAX_BUFF_SIZE];
};

class BGDummyClient
{
public:
	static HANDLE			s_hIocp;
	static SOCKADDR_IN		s_ServerAddr;
	static char				s_ip[20];

	static std::vector<BGDummyClient*> s_Clients;


	static void	Init();


public:
	BGDummyClient();
	~BGDummyClient();

	void	BitSet(int nBit);
	void	BitReset(int nBit);
	bool	BitIs(int nBit);
	
	bool	Connect(int);
	bool	Disconnect();
	bool	SendPacket(char* packet, WORD size);

	SOCKET	GetSocket() { return m_hSocket; }

	void	ProcessPacket(unsigned char* packet);


public:
	int				m_nId;
	long			m_nBit;

	SOCKET			m_hSocket;

	OVERLAPPED_EX	m_overExRecv;
	WSABUF			m_recv_wsa_buf;

	char			real_recv_buf[MAX_BUFF_SIZE];
	char			m_uncomplete_packet[MAX_BUFF_SIZE];
	int				m_current_packet_size;
	int				m_uncomplete_size;
};

