#pragma once

struct ServerAddress
{
	int		id;
	in_addr ip;
	int		port;
};
using ServerVector = std::vector<ServerAddress>;

enum class Server
{
	Test,
	Game
};

enum class ServerType
{
	InOffice,
	Qa,
	Live
};

class BGMainConfig
{
public:
	BGMainConfig() = default;
	~BGMainConfig() = default;

	static	bool						Open();
	static	void						Close();
	
public:
	static	std::string					s_strTitle;

	/** ��Ŀ ������ ���� */
	static	int							s_nNumberOfThreads;

	/** ���� ��� �ִ� ��*/
	static	int							s_nMaxUser;

	/** ���� ���� */
	static	ServerType					s_nServer;

	/** ���� �ܰ� (����, QA, ���̺�..) */
	static	Server						s_nServerType;

	/** ���� ��Ʈ*/
	static	UINT						s_nPort;
	
	/** ���� ���� */
	static	std::string					s_strNormalRedis;
	static	UINT						s_nNormalRedisPort;

	/** DB ���� */
	static	ServerVector				s_vDBServer;
	static	std::string					s_strNormalServer;
	static	UINT						s_nNormalPort;
	
	/** Other ���� ����*/
	static	ServerVector				s_vGameServer;
	static  std::string					s_strGameServer;
	static  UINT						s_nGamePort;

	static	ServerVector				s_vHttpServer;
	static  std::string					s_strHttpServer;
	static  UINT						s_nHttpPort;

	/** Client ���� */
	static UINT							s_nWantedProb;

	
	/** WhiteList ����*/
	static	bool						WhiteListIs(__int64 id);

	using 	WhiteListMap = std::map<__int64, __int64>;
	static	int							s_nWhiteListConnect;
	static	WhiteListMap				s_mapWhiteList;
};
