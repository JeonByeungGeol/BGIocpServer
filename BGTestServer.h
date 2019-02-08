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
		
	/** iocp ��ü�� port�� ���, BGIOServer::Start�� ȣ���մϴ�. */
	static	void				Start(int nLayerId);

	/** Accept��ȣ�� ���� ���Ŀ� BGIOServer::OnIOCallback ���� ȣ��˴ϴ�. */
	virtual	BGIOSocket*			CreateSocket(SOCKET newSocket, sockaddr_in* addr);

	/** ������ �ʿ� �߰��մϴ�.*/
	static	void				Add(BGTestSocket* pSocket);

	/** ������ �ʿ��� �����մϴ�. */
	static	void				Remove(BGTestSocket* pSocket);

	/** BGIOServer::Stop�� ȣ���ϰ�, ��� ���Ͽ� ���� CloseSocket�� ȣ���մϴ�.*/
	static	void				Stop();

	/** ��ϵ� ������ ������ Ȯ���մϴ�.*/
	static	int					Size();

	/** ���� �˻� */
	static	BGTestSocket*		FindSocket(long nId);

	/** ������ ã�� ���Ŀ�, Player�� �˻� */
	static	BGGameObject*		FindPlayer(long nId);

};

