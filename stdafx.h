// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// windows
#include <WinSock2.h>
#include <Windows.h>

// std
#include <iostream>
#include <fstream>
#include <string>
#include <strstream>

#include <map>
#include <queue>
#include <set>
#include <vector>
#include <list>

#include <mutex>
#include <shared_mutex>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
// 상수
#include "BGConst.h"

// 유틸
#include "BGSingleton.h"
#include "BGConfigManager.h"
#include "BGLogManager.h"

#pragma comment(lib, "ws2_32.lib")