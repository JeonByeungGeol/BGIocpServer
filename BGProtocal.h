#pragma once


/** ��Ŷ ũ�� �ִ� 256 */
using PacketSize = unsigned char;

/** ��Ŷ ���� �ִ� ~�� ���� ���� ���� */
enum class PacketType : unsigned short
{
	SC_PingTest,
	CS_PingTest,

	SC_Login,
	CS_Login
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

	__int64 id;
};

struct cs_packet_login {
	PacketSize size;
	PacketType type{ PacketType::CS_Login };

	char strId[30];
	char strPassward[30];
};




#pragma pack (pop)