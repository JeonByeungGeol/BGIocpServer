#pragma once

struct Position
{
	int x, y;
};

class BGTestPlayer
{
public:
	BGTestPlayer();
	~BGTestPlayer();

	int m_nTargetId;
	Position m_Position;
	std::set<long> m_setView;
	std::mutex m_VLLock;
};

