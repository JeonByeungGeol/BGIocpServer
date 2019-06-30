#include "stdafx.h"
#include "BGGameWorld.h"

#include "BGTestBasicMonster.h"
#include "BGTestPlayer.h"

BGGameWorld::BGGameWorld()
	: m_nNPCNum(BG_NPC_NUM)
{

}


BGGameWorld::~BGGameWorld()
{
}

bool BGGameWorld::Initialize(int id)
{
	m_nId = id;

	// 기본 NPC 생성 (추후에 데이터 읽어서 셋팅)
	for (int i = 0; i < m_nNPCNum; i++) {
		// worldId + objectId
		std::strstream nickStream;
		nickStream << "w:" << m_nId << ":" << i;

		BGTestBasicMonster *pMonster = new BGTestBasicMonster(m_nId);
		pMonster->SetName(nickStream.str());
		m_arrObject[i] = pMonster;
	}

	// 유저 객체 미리 생성
	for (int i = m_nNPCNum; i < BG_MAX_OBJECT_NUM; i++) {
		BGTestPlayer *pPlayer = new BGTestPlayer(m_nId);
		m_arrObject[i] = pPlayer;
	}

	BG_LOG_INFO("GameWorld Inialize success : %d", m_nId);
	return true;
}

BGTestPlayer * BGGameWorld::AddPlayer(BGTestSocket* pSocket)
{
	// 유저 객체 미리 생성
	for (int i = m_nNPCNum; i < BG_MAX_OBJECT_NUM; i++) {
		BGTestPlayer *pPlayer = static_cast<BGTestPlayer*>(m_arrObject[i]);
		pPlayer->Lock();
		if (pPlayer->isConnected()) {
			pPlayer->Unlock();
			continue;
		}
		else {
			if (pPlayer->Connect()) {
				pPlayer->Unlock();
				return pPlayer;
			}
			else {
				pPlayer->Unlock();
				continue;
			}
			
		}
	}

	BG_LOG_ERROR("Not exist extra player worldID=%d", m_nId);
	return nullptr;
}

