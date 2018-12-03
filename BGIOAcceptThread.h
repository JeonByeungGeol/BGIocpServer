#pragma once

#include "BGIOThread.h"

class BGIOObject;

class BGIOAcceptThread : public BGIOThread
{
public:
	BGIOAcceptThread(HANDLE hAcceptEvent, BGIOObject* pServer);
	virtual ~BGIOAcceptThread();

	/** ������ ���� �Լ��Դϴ�.*/
	virtual void			Run();

	/** �����尡 �����ϱ� ������ ȣ��Ǵ� �Լ��Դϴ�.*/
	virtual void			OnTerminate();



protected:
	/**
	 ù ��°�� selectevent�𵨿��� ����ϴ� �ڵ�
	 �� ��°�� �����̺�Ʈ
	*/
	HANDLE					m_vHandle[2];

	/** AcceptThread�� ������ ��ü(����)�Դϴ�. */
	BGIOObject*				m_pObject;
};

