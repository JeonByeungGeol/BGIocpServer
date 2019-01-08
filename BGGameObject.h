#pragma once

enum class ObjectType
{
	TEST_BASIC_MONSTER,
	TEST_PLAYER,
};

struct Position
{
	int x, y;
};

class BGGameObject
{
public:
	BGGameObject();
	~BGGameObject();

	


protected:
			__int64				m_nId;
			ObjectType			m_eObjectType;
			Position			m_Position;


			__int64				GetId() { return m_nId; }
	virtual	ObjectType			GetObjectType() = 0;
			Position			GetPosition() { return m_Position; }
};

