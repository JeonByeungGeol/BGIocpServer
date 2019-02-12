#include "stdafx.h"
#include "BGTestServer.h"

#include "BGMainConfig.h"
#include "BGLock.h"
#include "BGLayer.h"
#include "BGIOServer.h"

#include "BGTestSocket.h"

long							BGTestServer::s_nBit;
BGTestServer::SocketArr			BGTestServer::s_arrSocket;

static BGTestServer				g_server;
static BGRWLock					g_lock;
static int						g_nCount;

void BGTestServer::Start(int nLayerId)
{
	int nResult = g_server.BGIOServer::Start( BGLayer::GetIOCP(nLayerId), BGMainConfig::s_nPort);
	if (nResult == false) {
		BG_LOG_ERROR("BGIOServer Start Fail || nLayerId:%d, s_nPort:%u", nLayerId, BGMainConfig::s_nPort);
	}

	BG_LOG_DEBUG("%s Server Start(%u)", BGMainConfig::s_strTitle.c_str(), BGMainConfig::s_nPort);

	Initialize();
	BG_LOG_DEBUG("Initialize Success");
}

void BGTestServer::Initialize()
{

}

BGIOSocket* BGTestServer::CreateSocket(SOCKET newSocket, sockaddr_in* addr)
{
	int newId{ -1 };

	for (; ;){
		newId = -1;

		for (int i = 0; i < BG_MAX_CLIENT_NUM; i++) {
			if (!BGTestServer::s_arrSocket[i].BitIs(SOCKET_BIT_DISCONNECTED))
				continue;
			newId = i;
			break;
		}

		if (newId == -1) {
			BG_LOG_ERROR("Max User Connect");
			return nullptr;
		}
		
		// 소켓 잠금 이후에 다시 한번 확인
		BGTestSocket* pNewSocket = &BGTestServer::s_arrSocket[newId];
		pNewSocket->Lock();		
		if (pNewSocket->BitIs(SOCKET_BIT_DISCONNECTED)) {
			pNewSocket->BitReset(SOCKET_BIT_DISCONNECTED);
			pNewSocket->BitSet(SOCKET_BIT_CONNECTED);
			pNewSocket->m_nId = newId;
			pNewSocket->m_hSocket = newSocket;
			pNewSocket->m_nAddr = addr->sin_addr;
			pNewSocket->m_nPort = addr->sin_port;
			pNewSocket->AddRef();
			pNewSocket->Unlock();
			return pNewSocket;
		}
		pNewSocket->Unlock();		
	}	
}

void BGTestServer::Stop()
{
	g_server.BGIOServer::Stop();

	for (BGTestSocket& sock : s_arrSocket) {
		if(!sock.BitIs(SOCKET_BIT_CLOSED))
			sock.CloseSocket();
	}
	BG_LOG_DEBUG("%s Server Stop(%u)", BGMainConfig::s_strTitle.c_str(), BGMainConfig::s_nPort);
}

int BGTestServer::Size()
{
	return g_nCount;
}

BGTestSocket* BGTestServer::FindSocket(long nId)
{
	BGTestSocket* pSocket{ nullptr };
	pSocket = &s_arrSocket[nId];
		
	return pSocket;
}

BGGameObject* BGTestServer::FindPlayer(long nId)
{
	BGTestSocket* pSocket = FindSocket(nId);

	if (pSocket == nullptr)
		return nullptr;


	auto pGameObject = pSocket->m_pGameObject;
	// pGameObject->AddRef(&pGameObject->m_nFindRef);

	return pGameObject;
}