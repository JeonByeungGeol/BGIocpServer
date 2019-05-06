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
	 * ���� ���忡�� �����ϴ� player ��ü�� ��� ������ ��ü�� ��ȯ�Ѵ�.
	 * objectID�� ���⼭ �����ȴ�.
	*/
	BGTestPlayer* AddPlayer(BGTestSocket* pSocket);

private:
			int				m_nId;
	const	int				m_nNPCNum;
			ObjectArr		m_arrObject;
};

