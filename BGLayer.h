#pragma once

#include "BGLock.h"

class BGIOCompletionHandler;

/**
 * BGLayer 按眉 汲疙
 * 
 * BGServer甫 包府窍扁 困茄 鞘荐利牢 按眉甸阑 包府
 * 
 * BGIOWorkerThread 积己 棺 包府
 * BGIOCompletrion 积己 棺 包府
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

