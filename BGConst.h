#pragma once

/** 최대 동접 수 */
#define BG_MAX_CLIENT_NUM		10000

/** NPC 시작 인덱스 */
#define BG_NPC_START_INDEX		BG_MAX_CLIENT_NUM + 1

/** NPC 수 */
#define BG_NPC_NUM				100000

/** 동접 + NPC 최대 수*/
#define BG_MAX_OBJECT_NUM		BG_MAX_CLIENT_NUM + BG_NPC_NUM

/** 설정 파일 이름 */
static const char* const BG_CONFIG_FILE_NAME = "bg_config.ini";

/** IO 버퍼 크기*/
static const int BG_BUFFER_SIZE = 4096;


/** 서버 포트*/
static const int BG_SERVER_PORT = 8800;
