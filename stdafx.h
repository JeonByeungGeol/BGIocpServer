// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
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


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
// ���
#include "BGConst.h"

// ��ƿ
#include "BGSingleton.h"
#include "BGConfigManager.h"
#include "BGLogManager.h"

#pragma comment(lib, "ws2_32.lib")