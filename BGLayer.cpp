#include "stdafx.h"
#include "BGLayer.h"
#include "BGIOCompletionHandler.h"
#include "BGIOWorkerThread.h"

long					BGLayer::s_nId;
BGLayer::LayerMap		BGLayer::s_mapLayer;
BGRWLock				BGLayer::s_lock;


BGLayer::BGLayer()
	: m_nId(0), m_pIOCPHandler(nullptr)
{
}

BGLayer::~BGLayer()
{
	if (m_pIOCPHandler)
		delete m_pIOCPHandler;
}

int BGLayer::Add(int nThreadCount)
{
	BGLayer* pLayer = new BGLayer;
	pLayer->m_nId = InterlockedIncrement(&s_nId);
	pLayer->m_pIOCPHandler = new BGIOCompletionHandler;

	pLayer->m_pIOCPHandler->Init();


	BGIOWorkerThread* pThread;
	for (int nCount = 0; nCount < nThreadCount; nCount++)
	{
		pThread = new BGIOWorkerThread;
		pThread->Begin(pLayer->m_pIOCPHandler);
	}

	s_lock.WriteLock();
	s_mapLayer.insert(LayerMap::value_type(pLayer->m_nId, pLayer));
	s_lock.WriteUnlock();

	return pLayer->m_nId;
}

void BGLayer::Clear()
{
	s_lock.WriteLock();
	for (LayerMap::iterator it = s_mapLayer.begin(); it != s_mapLayer.end(); it++)
		delete (BGLayer*)(it->second);
	s_mapLayer.clear();
	s_lock.WriteUnlock();
}

BGIOCompletionHandler* BGLayer::GetIOCP(int nId)
{
	BGIOCompletionHandler* pHandler{ nullptr };

	s_lock.ReadLock();
	LayerMap::iterator it = s_mapLayer.find(nId);
	if (it != s_mapLayer.end())
		pHandler = it->second->m_pIOCPHandler;
	s_lock.ReadUnlock();

	return pHandler;
}