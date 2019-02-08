#include "stdafx.h"
#include "BGTestPlayer.h"

#include "BGTestSocket.h"

BGTestPlayer::BGTestPlayer(long nId, std::string strNickName, BGTestSocket* pSocket)
	: BGGameObject(nId, ObjectType::PLAYER, strNickName)
{
	m_pSocket				= pSocket;
	m_pSocket->AddRef();

}

BGTestPlayer::~BGTestPlayer()
{
	if (m_pSocket)
		m_pSocket->Release();
}
