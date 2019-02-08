#pragma once

#include "BGIOServer.h"

class BGTestSocket;
class BGGameObject;

class BGTestServer : public BGIOServer
{
public:
	using SocketArr = BGTestSocket[BG_MAX_CLIENT_NUM];
	using SocketMap = std::map<int, BGTestSocket*>;

	static	long		s_nServerBit;
	static	SocketMap	s_mapSocket;



public:
	static	void				BitSet(int nBit) {}
	static	void				BitReset(int nBit) {}
	static	bool				BitIs(int nBit) {}
	static	bool				ServerOnIs() {}
	static	bool				ServerStartIs() {}
		
	/** iocp 객체와 port를 얻어, BGIOServer::Start를 호출합니다. */
	static	void				Start(int nLayerId);

	/** Accept신호가 들어온 이후에 BGIOServer::OnIOCallback 에서 호출됩니다. */
	virtual	BGIOSocket*			CreateSocket(SOCKET newSocket, sockaddr_in* addr);

	/** 소켓을 맵에 추가합니다.*/
	static	void				Add(BGTestSocket* pSocket);

	/** 소켓을 맵에서 제거합니다. */
	static	void				Remove(BGTestSocket* pSocket);

	/** BGIOServer::Stop을 호출하고, 모든 소켓에 대해 CloseSocket을 호출합니다.*/
	static	void				Stop();

	/** 등록된 소켓의 갯수를 확인합니다.*/
	static	int					Size();

	/** 소켓 검색 */
	static	BGTestSocket*		FindSocket(long nId);

	/** 소켓을 찾은 이후에, Player를 검색 */
	static	BGGameObject*		FindPlayer(long nId);

};

