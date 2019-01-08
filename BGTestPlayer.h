#pragma once

#include "BGGameObject.h"

class BGTestSocket;


class BGTestPlayer : public BGGameObject
{
	friend	class					BGTestSocket;
			BGTestSocket*			m_pSocket;
public:
	BGTestPlayer(BGTestSocket* pSocket, __int64 n64UID, std::string strNickName);
	~BGTestPlayer();


public:
	ObjectType						GetObjectType() { return ObjectType::TEST_PLAYER; }

	__int64							m_n64UID;
	std::string						m_strNickName;
	int								m_nTargetId;
	std::set<long>					m_setView;
	std::mutex						m_VLLock;
};

