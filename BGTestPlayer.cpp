#include "stdafx.h"
#include "BGTestPlayer.h"

#include "BGTestSocket.h"

BGTestPlayer::BGTestPlayer(int nGameWorIdIndex, BGTestSocket* pSocket)
	: BGGameObject(nGameWorIdIndex, ObjectType::PLAYER)
{
	m_bConnected			= false;
	m_pSocket				= pSocket;
	m_pSocket->AddRef();

}

BGTestPlayer::BGTestPlayer(int nGameWorIdIndex)
	: BGGameObject(nGameWorIdIndex, ObjectType::PLAYER)
{
	m_bConnected = false;
}

BGTestPlayer::~BGTestPlayer()
{
	if (m_pSocket)
		m_pSocket->Release();
}

bool BGTestPlayer::Connect()
{
	if (nullptr == m_pSocket)
		return false;
	
	m_bConnected = true;
	return true;
}

void BGTestPlayer::Disconnect()
{
	if (isConnected() && m_pSocket == nullptr) {
		BG_LOG_ERROR("1 objectId=%d", GetId());
	}

	if(!isConnected() && m_pSocket != nullptr) {
		BG_LOG_ERROR("2 socketId=%d", m_pSocket->Id());
	}

	m_bConnected = false;

	if (m_pSocket)
		m_pSocket->Release();

}

void BGTestPlayer::SetSocket(BGTestSocket* pSocket) {
	if (m_pSocket)
		m_pSocket->Release();

	m_pSocket = pSocket;
	m_pSocket->AddRef();
}
