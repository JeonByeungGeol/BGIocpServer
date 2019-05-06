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

		BGTestBasicMonster *pMonster = new BGTestBasicMonster(i, name);		
		m_arrObject[i] = pMonster;
	}

	// ���� ��ü �̸� ����
	for (int i = m_nNPCNum; i < BG_MAX_OBJECT_NUM; i++) {
		std::string name{ "player:" };
		name.append(std::to_string(m_nId));
		name.append(":");
		name.append(std::to_string(i));

		BGTestPlayer *pMonster = new BGTestPlayer(i, name);
		m_arrObject[i] = pMonster;
	}

	BG_LOG_INFO("GameWorld Inialize success : %d", m_nId);
	return true;
}

