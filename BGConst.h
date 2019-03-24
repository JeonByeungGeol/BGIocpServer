#pragma once

/** �ִ� ���� �� */
#define BG_MAX_CLIENT_NUM		10000

/** NPC ���� �ε��� */
#define BG_NPC_START_INDEX		BG_MAX_CLIENT_NUM + 1

/** NPC �� */
#define BG_NPC_NUM				100000

/** ���� + NPC �ִ� ��*/
#define BG_MAX_OBJECT_NUM		BG_MAX_CLIENT_NUM + BG_NPC_NUM

/** ���� ���� �̸� */
static const char* const BG_CONFIG_FILE_NAME = "bg_config.ini";

/** IO ���� ũ��*/
static const int BG_BUFFER_SIZE = 4096;


/** ���� ��Ʈ*/
static const int BG_SERVER_PORT = 8800;
