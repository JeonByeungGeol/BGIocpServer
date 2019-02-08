#pragma once

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


class BGGameObject
{
	friend	class					BGTestSocket;
public:
	BGGameObject(long nId, ObjectType objectType = ObjectType::NONE, std::string strNickName = "NONAME");
	~BGGameObject();
	
protected:
			/** 소켓 아이디와 동일*/
			long						m_nId;
			
			/** 객체 타입*/
			ObjectType					m_eObjectType;

			/** 위치 정보*/
			Position					m_Position;

			/** 객체 이름*/
			std::string					m_strNickName;

			/** 시야 리스트 */
			std::set<long>				m_setView;
			std::mutex					m_VLLock;

			/** 타켓 ID*/
			int							m_nTargetId;

public:
	long						GetId() { return m_nId; }
			virtual	ObjectType			GetObjectType() { return m_eObjectType; }
			Position					GetPosition() { return m_Position; }
			std::string					GetName() { return m_strNickName; }
};

