#pragma once

class BGTestSocket;

struct Position
{
	int x, y;
};

class BGTestPlayer
{
	friend	class					BGTestSocket;
			BGTestSocket*			m_pSocket;
public:
	BGTestPlayer(BGTestSocket* pSocket, __int64 n64UID, std::string strNickName);
	~BGTestPlayer();

	__int64							m_n64UID;

	std::string						m_strNickName;
	int								m_nTargetId;
	Position						m_Position;
	std::set<long>					m_setView;
	std::mutex						m_VLLock;
};

