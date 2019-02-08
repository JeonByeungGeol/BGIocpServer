#include "stdafx.h"
#include "BGTestBasicMonster.h"


BGTestBasicMonster::BGTestBasicMonster(long nId, std::string strNickName)
	: BGGameObject(nId, ObjectType::TEST_BASIC_MONSTER, strNickName)
{

}


BGTestBasicMonster::~BGTestBasicMonster()
{
}
