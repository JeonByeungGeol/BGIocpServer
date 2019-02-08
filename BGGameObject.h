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
			/** ���� ���̵�� ����*/
			long						m_nId;
			
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
	long						GetId() { return m_nId; }
			virtual	ObjectType			GetObjectType() { return m_eObjectType; }
			Position					GetPosition() { return m_Position; }
			std::string					GetName() { return m_strNickName; }
};

