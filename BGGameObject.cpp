#include "stdafx.h"
#include "BGGameObject.h"


BGGameObject::BGGameObject(long nId, ObjectType objectType, std::string strNickName)
	: m_nId(nId), m_eObjectType(objectType), m_strNickName(strNickName)
	, m_nTargetId(0)
{
	m_setView.clear();
}


BGGameObject::~BGGameObject()
{
}
