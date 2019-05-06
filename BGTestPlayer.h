#pragma once

#include "BGGameObject.h"

class BGTestSocket;


class BGTestPlayer : public BGGameObject
{
public:
	BGTestPlayer(int nGameWorIdIndex, BGTestSocket* pSocket);
	BGTestPlayer(int nGameWorIdIndex);
	~BGTestPlayer();

public:
	bool isConnected() { return m_bConnected; }
	bool Connect();
	void Disconnect();


private:
	/** ���� �������� ����*/
	bool							m_bConnected;

	BGTestSocket*					m_pSocket;


public:
	void SetSocket(BGTestSocket* pSocket);
};