#include "stdafx.h"
#include "BGTestServer.h"

#include "BGMainConfig.h"
#include "BGLock.h"
#include "BGLayer.h"
#include "BGIOServer.h"

#include "BGTestSocket.h"
#include "BGGameWorld.h"

long							BGTestServer::s_nBit;
BGTestServer::SocketArr			BGTestServer::s_arrSocket;
BGTestServer::GameWorldArr		BGTestServer::s_arrGameWorld;

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
	int maxNum{ BG_MAX_CLIENT_NUM };
	int maxSize{ sizeof(s_arrSocket) };
	for (BGTestSocket sock: s_arrSocket) {
		sock.AddRef();
	}

	BG_LOG_INFO("Max Client Num : %d, Max Client Size %d Mb.", maxNum, maxSize / (1000 * 1000));

	int index{ 0 };
	for (BGGameWorld object : s_arrGameWorld) {
		object.Initialize(index++);
	}
	BG_LOG_INFO("GameWorld Initialize Complete Num : %d, Max GameWorld Size %d Mb."
		, sizeof(s_arrGameWorld) / sizeof(BGGameWorld), sizeof(s_arrGameWorld) / (1000 * 1000));
}

BGIOSocket* BGTestServer::CreateSocket(SOCKET newSocket, sockaddr_in* addr)
{
	int newId{ -1 };

	for (int i = 0; i < s_createSocketRetryCnt; i++) {
		newId = -1;

		for (int i = 0; i < BG_MAX_CLIENT_NUM; i++) {
			if (!BGTestServer::s_arrSocket[i].BitIs(SOCKET_BIT_NOT_USED))
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
		if (pNewSocket->BitIs(SOCKET_BIT_NOT_USED)) {

			if (!pNewSocket->CheckResetData()) {
				// 에러로그를 남기고 강제로 초기화 시켜준다.
				pNewSocket->ResetDataOnForce();
				pNewSocket->Unlock();
				BG_LOG_ERROR("CheckResetData is Fail index=%d", newId);
				continue;
			}

			pNewSocket->BitReset(SOCKET_BIT_NOT_USED);
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

	/*
		'소켓 배정하려는 사이에 변경이 일어남' 이 경우가
		최대 횟수만큼 도달한 경우 무한루프 빠져나옴
		보통의 경우에는 해당 로그가 찍히면 안됨
	*/	
	BG_LOG_ERROR("Limit Retry Count.");
	return nullptr;
}

void BGTestServer::Stop()
{
	g_server.BGIOServer::Stop();

	for (BGTestSocket& sock : s_arrSocket) {
		if(sock.BitIs(SOCKET_BIT_CONNECTED))
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

BGTestPlayer* BGTestServer::FindPlayer(long nId)
{
	BGTestSocket* pSocket = FindSocket(nId);

	if (pSocket == nullptr)
		return nullptr;


	auto pGameObject = pSocket->m_pPlayer;
	// pGameObject->AddRef(&pGameObject->m_nFindRef);

	return pGameObject;
}

BGGameWorld* BGTestServer::FindGameWorld(int nId)
{
	if (BG_MAX_GAME_WORLD_NUM <= nId) {
		BG_LOG_ERROR("arr bind err index=%d", nId);
		return nullptr;
	}

	return &s_arrGameWorld[nId];
}

void BGTestServer::Post(int nId, BGIOObject * pObject)
{
	BGLayer::GetIOCP(BGTestSocket::s_nNetworkId)->Post(nId, pObject);
}
