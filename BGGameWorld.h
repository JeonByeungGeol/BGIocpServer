#pragma once

#include "BGGameObject.h"

class BGTestPlayer;

class BGGameWorld
{
public:
	using ObjectArr = BGGameObject * [BG_MAX_OBJECT_NUM];


public:
	BGGameWorld();
	~BGGameWorld();

public:
	bool Initialize(int id);

	/** 
	 * 게임 월드에서 재사용하는 player 객체중 사용 가능한 객체를 반환한다.
	 * objectID가 여기서 고정된다.
	*/
	BGTestPlayer* AddPlayer(BGTestSocket* pSocket);

private:
			int				m_nId;
	const	int				m_nNPCNum;
			ObjectArr		m_arrObject;
};

