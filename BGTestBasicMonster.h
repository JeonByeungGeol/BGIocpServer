#pragma once

#include "BGGameObject.h"

class BGTestBasicMonster : public BGGameObject
{
public:
	BGTestBasicMonster(int nGameWorIdIndex);
	~BGTestBasicMonster();

public:
	virtual void OnTimer(int nId);
};

