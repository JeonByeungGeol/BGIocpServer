#pragma once

class BGIOObject;

class BGIOTimer
{
public:
	BGIOTimer(BGIOObject *pObject, __int64 n64Time, int nId = 0)
		: m_n64Time(n64Time), m_pObject(pObject), m_nId(nId) {}

	/** 오름차순 */
	bool operator<(const BGIOTimer& rTimer) const
	{
		return (m_n64Time - rTimer.m_n64Time) > 0;
	}

public:
	/** 동작시킬 객체*/
	BGIOObject*		m_pObject;

	/** 동작할 시간*/
	__int64			m_n64Time;

	/** 객체에 정의된 타이머 동작중 어떤 동작을 실행할지에 대한 값*/
	int				m_nId;
};