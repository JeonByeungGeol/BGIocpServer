#pragma once

#include "BGIOThread.h"

class BGIOObject;

class BGIOAcceptThread : public BGIOThread
{
public:
	BGIOAcceptThread(HANDLE hAcceptEvent, BGIOObject* pServer);
	virtual ~BGIOAcceptThread();

	/** ������ ���� �Լ��Դϴ�.*/
	virtual void Run();

	/** �����尡 �����ϱ� ������ ȣ��Ǵ� �Լ��Դϴ�.*/
	virtual void OnTerminate();

protected:
	/** ù ��°�� �����̺�Ʈ, �� ��°�� selectevent�𵨿��� ����ϴ� �ڵ��Դϴ�.*/
	HANDLE m_vHandle[2];

	/** �θ� ���� ��ü�Դϴ�*/
	BGIOObject* m_pObject;
};

