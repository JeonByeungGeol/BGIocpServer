#pragma once

#include "BGIOServer.h"

class BGTestSocket;
class BGGameWorld;
class BGTestPlayer;

class BGTestServer : public BGIOServer
{
public:
	using SocketArr = BGTestSocket[BG_MAX_CLIENT_NUM];
	using GameWorldArr = BGGameWorld[BG_MAX_GAME_WORLD_NUM];

	static	long			s_nBit;
	static SocketArr		s_arrSocket;
	static GameWorldArr		s_arrGameWorld;

	// createSocket���� ��õ��� Ƚ��
	static const int	s_createSocketRetryCnt{ 3 };

public:
	static	void				BitSet(int nBit) { InterlockedExchange(&s_nBit, s_nBit | nBit); }
	static	void				BitReset(int nBit) { InterlockedExchange(&s_nBit, s_nBit & ~nBit); }
	static	bool				BitIs(int nBit) { return ((s_nBit & nBit) == nBit) ? true : false; }
	static	bool				ServerOnIs() {}
	static	bool				ServerStartIs() {}


	/** iocp ��ü�� port�� ���, BGIOServer::Start�� ȣ���մϴ�. */
	static	void				Start(int nLayerId);

	/** 
	  * Startȣ�� ���Ŀ�, �ʿ��� �ʱ�ȭ �۾��� �����մϴ�.
	  * ���� ���� �ʱ�ȭ	  
	*/
	static void					Initialize();

	/**
	  Accept��ȣ�� ���� ���Ŀ� BGIOServer::OnIOCallback ���� ȣ��˴ϴ�.
	  s_arrSocket���� ������� �ʴ� ������ ã�� ��ȯ�մϴ�.
	*/
	virtual	BGIOSocket*			CreateSocket(SOCKET newSocket, sockaddr_in* addr);

	/** BGIOServer::Stop�� ȣ���ϰ�, ��� ���Ͽ� ���� CloseSocket�� ȣ���մϴ�.*/
	static	void				Stop();

	/** ��ϵ� ������ ������ Ȯ���մϴ�.*/
	static	int					Size();

	/** ���� �˻� */
	static	BGTestSocket*		FindSocket(long nId);

	/** ������ ã�� ���Ŀ�, Player�� �˻� */
	static	BGTestPlayer*		FindPlayer(long nId);

	/** ���� ���� �˻� */
	static BGGameWorld*			FindGameWorld(int nId);
};

