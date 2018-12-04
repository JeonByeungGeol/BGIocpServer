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

	/** 워커 스레드 갯수 */
	static	int							s_nNumberOfThreads;

	/** 접속 허용 최대 값*/
	static	int							s_nMaxUser;

	/** 서버 종류 */
	static	ServerType					s_nServer;

	/** 서버 단계 (개발, QA, 라이브..) */
	static	Server						s_nServerType;

	/** 서버 포트*/
	static	UINT						s_nPort;
	
	/** 레디스 서버 */
	static	std::string					s_strNormalRedis;
	static	UINT						s_nNormalRedisPort;

	/** DB 서버 */
	static	ServerVector				s_vDBServer;
	static	std::string					s_strNormalServer;
	static	UINT						s_nNormalPort;
	
	/** Other 서버 정보*/
	static	ServerVector				s_vGameServer;
	static  std::string					s_strGameServer;
	static  UINT						s_nGamePort;

	static	ServerVector				s_vHttpServer;
	static  std::string					s_strHttpServer;
	static  UINT						s_nHttpPort;

	/** Client 관련 */
	static UINT							s_nWantedProb;

	
	/** WhiteList 관리*/
	static	bool						WhiteListIs(__int64 id);

	using 	WhiteListMap = std::map<__int64, __int64>;
	static	int							s_nWhiteListConnect;
	static	WhiteListMap				s_mapWhiteList;
};
