#pragma once

#include "BGLock.h"

class BGIOCompletionHandler;

/**
 * BGLayer ��ü ����
 * 
 * BGServer�� �����ϱ� ���� �ʼ����� ��ü���� ����
 * 
 * BGIOWorkerThread ���� �� ����
 * BGIOCompletrion ���� �� ����
 *
*/

class BGLayer
{
public:
	using LayerMap = std::map<int, BGLayer*>;

	static	int								Add(int nThreadCount = 1);
	static	void							Clear();
	static	BGIOCompletionHandler*			GetIOCP(int nId);

	static	LayerMap						s_mapLayer;
	static	long							s_nId;
	static	BGRWLock						s_lock;


public:
	BGLayer();
	~BGLayer();

public:
	int										m_nId;
	BGIOCompletionHandler*					m_pIOCPHandler;
};

