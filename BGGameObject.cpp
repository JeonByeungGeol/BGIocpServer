#include "stdafx.h"
#include "BGGameObject.h"


BGGameObject::BGGameObject(int nGameWorIdIndex, ObjectType objectType)
	: m_nGameWorIdIndex(nGameWorIdIndex), m_eObjectType(objectType), m_strNickName("NONAME")
	, m_nTargetId(0)
{
	m_setView.clear();
}


BGGameObject::~BGGameObject()
{
}
