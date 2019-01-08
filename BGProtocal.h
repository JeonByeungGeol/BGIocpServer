#pragma once


/** 패킷 크기 최대 256 */
using PacketSize = unsigned char;

/** 패킷 종류 최대 ~개 까지 정의 가능 */
enum class PacketType : unsigned short
{
	SC_PingTest,
	CS_PingTest,

	SC_Login,
	CS_Login,

	SC_Put_Object,
	SC_Remove_Object
};




#pragma pack (push, 1)
struct packet_basic_protocal {
	PacketSize size;
	PacketType type;
};

struct sc_packet_ping_test {
	PacketSize size;
	PacketType type{ PacketType::SC_PingTest };
};

struct cs_packet_ping_test {
	PacketSize size;
	PacketType type{ PacketType::CS_PingTest };
};

struct sc_packet_login {
	PacketSize size;
	PacketType type{ PacketType::SC_Login };

	__int64 client_id;
	__int64 object_id;
};

struct cs_packet_login {
	PacketSize size;
	PacketType type{ PacketType::CS_Login };

	char strId[30];
	char strPassward[30];
};


struct sc_packet_put_object {
	PacketSize size;
	PacketType type{ PacketType::SC_Put_Object };

	unsigned char object_type;
	__int64 object_id;
	int x, y;
};

struct sc_packet_remove_object {
	PacketSize size;
	PacketType type{ PacketType::SC_Remove_Object };

	__int64 object_id;
};



#pragma pack (pop)