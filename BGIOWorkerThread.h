#pragma once

#include "BGIOThread.h"
class BGIOWorkerThread : public BGIOThread
{
public:
	static	long			s_nRunningThread;

public:
	BGIOWorkerThread();
	virtual ~BGIOWorkerThread();
	
	virtual	void			Run();
	virtual	void			OnTerminate();
};

