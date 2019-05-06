#pragma once

#include "BGGameObject.h"


class BGGameWorld
{
public:
	using ObjectArr = BGGameObject * [BG_MAX_OBJECT_NUM];


public:
	BGGameWorld();
	~BGGameWorld();

public:
	bool Initialize(int id);


private:
			int				m_nId;
	const	int				m_nNPCNum;
			ObjectArr		m_arrObject;
};

