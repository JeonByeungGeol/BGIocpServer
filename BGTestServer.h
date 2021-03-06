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

	// createSocket에서 재시도할 횟수
	static const int	s_createSocketRetryCnt{ 3 };

public:
	static	void				BitSet(int nBit) { InterlockedExchange(&s_nBit, s_nBit | nBit); }
	static	void				BitReset(int nBit) { InterlockedExchange(&s_nBit, s_nBit & ~nBit); }
	static	bool				BitIs(int nBit) { return ((s_nBit & nBit) == nBit) ? true : false; }
	static	bool				ServerOnIs() {}
	static	bool				ServerStartIs() {}


	/** iocp 객체와 port를 얻어, BGIOServer::Start를 호출합니다. */
	static	void				Start(int nLayerId);

	/** 
	  * Start호출 이후에, 필요한 초기화 작업을 진행합니다.
	  * 게임 월드 초기화	  
	*/
	static void					Initialize();

	/**
	  Accept신호가 들어온 이후에 BGIOServer::OnIOCallback 에서 호출됩니다.
	  s_arrSocket에서 사용하지 않는 소켓을 찾아 반환합니다.
	*/
	virtual	BGIOSocket*			CreateSocket(SOCKET newSocket, sockaddr_in* addr);

	/** BGIOServer::Stop을 호출하고, 모든 소켓에 대해 CloseSocket을 호출합니다.*/
	static	void				Stop();

	/** 등록된 소켓의 갯수를 확인합니다.*/
	static	int					Size();

	/** 소켓 검색 */
	static	BGTestSocket*		FindSocket(long nId);

	/** 소켓을 찾은 이후에, Player를 검색 */
	static	BGTestPlayer*		FindPlayer(long nId);

	/** 게임 월드 검색 */
	static BGGameWorld*			FindGameWorld(int nId);

	/** IOCP POST 기능 (TimerThread - WorkerThread 연동)*/
	static void Post(int nId, BGIOObject* pObject);
};

