#include "stdafx.h"
#include "BGTestServer.h"

#include "BGMainConfig.h"
#include "BGLock.h"
#include "BGLayer.h"
#include "BGIOServer.h"

#include "BGTestSocket.h"

long							BGTestServer::s_nServerBit;
BGTestServer::SocketMap			BGTestServer::s_mapSocket;

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
}

BGIOSocket* BGTestServer::CreateSocket(SOCKET newSocket, sockaddr_in* addr)
{
	BGTestSocket* pSocket = new BGTestSocket(newSocket, addr);
	return pSocket;
}


void BGTestServer::Add(BGTestSocket* pSocket)
{
	g_lock.WriteLock();

	s_mapSocket.insert(SocketMap::value_type(pSocket->Id(), pSocket));
	g_nCount++;

	g_lock.WriteUnlock();
}

void BGTestServer::Remove(BGTestSocket* pSocket)
{
	g_lock.WriteLock();

	g_nCount--;
	s_mapSocket.erase(pSocket->Id());

	g_lock.WriteUnlock();
}

void BGTestServer::Stop()
{
	g_server.BGIOServer::Stop();

	g_lock.ReadLock();
	
	// pSocket->CloseSocket();

	g_lock.ReadUnlock();

	BG_LOG_DEBUG("%s Server Stop(%u)", BGMainConfig::s_strTitle.c_str(), BGMainConfig::s_nPort);
}

int BGTestServer::Size()
{
	return g_nCount;
}

BGTestSocket* BGTestServer::FindSocket(long nId)
{
	BGTestSocket* pSocket{ nullptr };

	g_lock.ReadLock();
	
	SocketMap::iterator it = s_mapSocket.find(nId);
	if (it != s_mapSocket.end())
	{
		pSocket = it->second;
		pSocket->AddRef();
	}

	g_lock.ReadUnlock();
	return pSocket;
}

BGTestPlayer* BGTestServer::FindPlayer(long nId)
{
	BGTestPlayer* pPlayer{ nullptr };

	g_lock.ReadLock();

	SocketMap::iterator it = s_mapSocket.find(nId);
	if (it != s_mapSocket.end())
	{
		BGTestSocket* pSocket = it->second;
		pSocket->Lock();
		pPlayer = pSocket->m_pPlayer;
		if (pPlayer)
		{
			//pPlayer->AddRef(&pPlayer->m_nFindRef);
		}
		pSocket->Unlock();		
	}
	g_lock.ReadUnlock();
	return pPlayer;
}