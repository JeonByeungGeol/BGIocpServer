#include "stdafx.h"
#include "BGTestBasicMonster.h"


BGTestBasicMonster::BGTestBasicMonster(int nGameWorIdIndex)
	: BGGameObject(nGameWorIdIndex, ObjectType::TEST_BASIC_MONSTER)
{

}


BGTestBasicMonster::~BGTestBasicMonster()
{
}

void BGTestBasicMonster::OnTimer(int nId)
{
	switch (nId)
	{
	case 50:
		BG_LOG_DEBUG("%d", nId);
		break;
	}
}