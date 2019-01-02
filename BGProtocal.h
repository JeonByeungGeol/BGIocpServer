#pragma once


/** 패킷 크기 최대 256*/
using PacketSize = unsigned char;

/** 패킷 종류 최대 256개 까지 정의 가능*/
using PacketType = unsigned char;


#pragma pack (push, 1)

struct pint_test {
	PacketSize size;
	PacketType type;
};

#pragma pack (pop)