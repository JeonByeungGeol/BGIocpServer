#pragma once

#include "BGGameObject.h"

class BGTestSocket;


class BGTestPlayer : public BGGameObject
{			
public:
	BGTestPlayer(long nId, std::string strNickName, BGTestSocket* pSocket);
	~BGTestPlayer();


public:


private:
			BGTestSocket*					m_pSocket;

};

