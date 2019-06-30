#pragma once

/*
 * 배열 인덱스로 사용할 값은 #DEFINE으로 정의
 * 나머지 상수는 static const로 정의
*/

/** 게임 월드 수 */
#define BG_MAX_GAME_WORLD_NUM	1

/** 최대 동접 수 */
#define BG_MAX_CLIENT_NUM		10000

/** BGGameWorld 당  NPC 수 */
#define BG_NPC_NUM				100000

/** BGGameWorld 당 동접 + NPC 최대 수*/
#define BG_MAX_OBJECT_NUM		BG_MAX_CLIENT_NUM + BG_NPC_NUM

/** 설정 파일 이름 */
static const char* const BG_CONFIG_FILE_NAME = "bg_config.ini";

/** IO 버퍼 크기*/
static const int BG_BUFFER_SIZE = 4096;

/** 서버 포트*/
static const int BG_SERVER_PORT = 8800;
