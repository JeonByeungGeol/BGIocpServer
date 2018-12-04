#include "stdafx.h"
#include "BGMainConfig.h"

std::string							BGMainConfig::s_strTitle;
int									BGMainConfig::s_nNumberOfThreads;
int									BGMainConfig::s_nMaxUser;
ServerType							BGMainConfig::s_nServer;
Server								BGMainConfig::s_nServerType;
UINT								BGMainConfig::s_nPort;
std::string							BGMainConfig::s_strNormalRedis;
UINT								BGMainConfig::s_nNormalRedisPort;
ServerVector						BGMainConfig::s_vDBServer;
std::string							BGMainConfig::s_strNormalServer;
UINT								BGMainConfig::s_nNormalPort;
ServerVector						BGMainConfig::s_vGameServer;
std::string							BGMainConfig::s_strGameServer;
UINT								BGMainConfig::s_nGamePort;
ServerVector						BGMainConfig::s_vHttpServer;
std::string							BGMainConfig::s_strHttpServer;
UINT								BGMainConfig::s_nHttpPort;
UINT								BGMainConfig::s_nWantedProb;
int									BGMainConfig::s_nWhiteListConnect;
BGMainConfig::WhiteListMap			BGMainConfig::s_mapWhiteList;



bool BGMainConfig::Open()
{
	if (false == g_Config.Load()) {
		std::cout << "Config Load Fail" << std::endl;
		return false;
	}

	s_strTitle = g_Config.GetString("serverName");

	s_nNumberOfThreads = g_Config.GetInt("workerThreadNum");

	return true;
}

void BGMainConfig::Close()
{
}
