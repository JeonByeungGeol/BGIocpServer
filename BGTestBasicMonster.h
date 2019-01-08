#pragma once

#include "BGGameObject.h"

class BGTestBasicMonster : public BGGameObject
{
public:
	BGTestBasicMonster();
	~BGTestBasicMonster();

public:
	virtual	ObjectType					GetObjectType() { return ObjectType::TEST_BASIC_MONSTER; }

			int								m_nTargetId;

			std::set<long>					m_setView;
			std::mutex						m_VLLock;
};

