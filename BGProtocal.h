#pragma once


/** ��Ŷ ũ�� �ִ� 256*/
using PacketSize = unsigned char;

/** ��Ŷ ���� �ִ� 256�� ���� ���� ����*/
using PacketType = unsigned char;


#pragma pack (push, 1)

struct pint_test {
	PacketSize size;
	PacketType type;
};

#pragma pack (pop)