#include "stdafx.h"
#include "BGTestOtherSocket.h"

#include "BGMainConfig.h"

BGTestOtherSocket::BGTestOtherSocket()
	: BGIOSocket(0)
{
}


BGTestOtherSocket::~BGTestOtherSocket()
{
}


bool BGTestOtherSocket::Start()
{
	bool bFail{ false };

	for (auto address : BGMainConfig::s_vTestServer) {
		if (!Create(address.id, address.ip, address.port))
			bFail = true;

		SwitchToThread();
	}

	if (bFail) {

	}
	else {
		BG_LOG_INFO("Start");
	}

	return true;
}