#include "stdafx.h"
#include "BGTestPlayer.h"

#include "BGTestSocket.h"

BGTestPlayer::BGTestPlayer(BGTestSocket * pSocket, __int64 n64UID, std::string strNickName)
{
	m_pSocket				= pSocket;
	m_pSocket->AddRef();

	m_n64UID				= n64UID;
	m_strNickName			= strNickName;

	m_nTargetId				= 0;
	m_setView.clear();


}

BGTestPlayer::~BGTestPlayer()
{
	if (m_pSocket)
		m_pSocket->Release();
}
