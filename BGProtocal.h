#pragma once


/** 패킷 크기 최대 256 */
using PacketSize = unsigned char;

/** 패킷 종류 최대 ~개 까지 정의 가능 */
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