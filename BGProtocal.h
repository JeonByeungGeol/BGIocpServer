#pragma once


/** ��Ŷ ũ�� �ִ� 256 */
using PacketSize = unsigned char;

/** ��Ŷ ���� �ִ� ~�� ���� ���� ���� */
enum class PacketType : unsigned short
{
	SC_PingTest,
	CS_PingTest,
};




#pragma pack (push, 1)
struct sc_packet_ping_test {
	PacketSize size;
	PacketType type{ PacketType::SC_PingTest };
};

struct cs_packet_ping_test {
	PacketSize size;
	PacketType type{ PacketType::CS_PingTest };
};





#pragma pack (pop)