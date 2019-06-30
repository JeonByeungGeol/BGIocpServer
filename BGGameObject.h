#pragma once

#include "BGLock.h"
#include "BGIOObject.h"

enum class ObjectType : unsigned char
{
	TEST_BASIC_MONSTER,
	PLAYER,
	NONE,
};

struct Position
{
	int x, y;
};


class BGGameObject : public BGIOObject
{
	friend	class					BGTestSocket;
public:
	BGGameObject(int nGameWorIdIndex, ObjectType objectType = ObjectType::NONE);
	~BGGameObject();

protected:
	BGIOCriticalSection			m_lock;

public:
	void						Lock() { m_lock.Enter(); }
	void						Unlock() { m_lock.Leave(); }

protected:
	/** GameWorld ID*/
	int							m_nGameWorIdIndex;

	/** GameWorld ���� Object ID*/
	int							m_nObjectId;

	/** ��ü Ÿ��*/
	ObjectType					m_eObjectType;

	/** ��ġ ����*/
	Position					m_Position;

	/** ��ü �̸�*/
	std::string					m_strNickName;

	/** �þ� ����Ʈ */
	std::set<long>				m_setView;
	std::mutex					m_VLLock;

	/** Ÿ�� ID*/
	int							m_nTargetId;

public:
	int							GetWorldId() { return m_nGameWorIdIndex; }
	long						GetId() { return m_nObjectId; }
	virtual	ObjectType			GetObjectType() { return m_eObjectType; }
	Position					GetPosition() { return m_Position; }
	std::string					GetName() { return m_strNickName; }

	void						SetName(std::string nick) { m_strNickName = nick; }
};

