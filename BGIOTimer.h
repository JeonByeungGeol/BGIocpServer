#pragma once

class BGIOObject;

class BGIOTimer
{
public:
	BGIOTimer(BGIOObject *pObject, __int64 n64Time, int nId = 0)
		: m_n64Time(n64Time), m_pObject(pObject), m_nId(nId) {}

	/** �������� */
	bool operator<(const BGIOTimer& rTimer) const
	{
		return (m_n64Time - rTimer.m_n64Time) > 0;
	}

public:
	/** ���۽�ų ��ü*/
	BGIOObject*		m_pObject;

	/** ������ �ð�*/
	__int64			m_n64Time;

	/** ��ü�� ���ǵ� Ÿ�̸� ������ � ������ ���������� ���� ��*/
	int				m_nId;
};