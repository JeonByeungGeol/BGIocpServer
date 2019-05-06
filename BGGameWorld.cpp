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

	// �⺻ NPC ���� (���Ŀ� ������ �о ����)
	for (int i = 0; i < m_nNPCNum; i++) {
		std::string name{ "test:" };
		name.append(std::to_string(m_nId));
		name.append(":");
		name.append(std::to_string(i));

		BGTestBasicMonster *pMonster = new BGTestBasicMonster(m_nId);
		pMonster->SetName(name);
		m_arrObject[i] = pMonster;
	}

	// ���� ��ü �̸� ����
	for (int i = m_nNPCNum; i < BG_MAX_OBJECT_NUM; i++) {
		BGTestPlayer *pPlayer = new BGTestPlayer(m_nId);
		m_arrObject[i] = pPlayer;
	}

	BG_LOG_INFO("GameWorld Inialize success : %d", m_nId);
	return true;
}

BGTestPlayer * BGGameWorld::AddPlayer(BGTestSocket* pSocket)
{
	// ���� ��ü �̸� ����
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

