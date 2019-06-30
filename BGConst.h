#pragma once

/*
 * �迭 �ε����� ����� ���� #DEFINE���� ����
 * ������ ����� static const�� ����
*/

/** ���� ���� �� */
#define BG_MAX_GAME_WORLD_NUM	1

/** �ִ� ���� �� */
#define BG_MAX_CLIENT_NUM		10000

/** BGGameWorld ��  NPC �� */
#define BG_NPC_NUM				100000

/** BGGameWorld �� ���� + NPC �ִ� ��*/
#define BG_MAX_OBJECT_NUM		BG_MAX_CLIENT_NUM + BG_NPC_NUM

/** ���� ���� �̸� */
static const char* const BG_CONFIG_FILE_NAME = "bg_config.ini";

/** IO ���� ũ��*/
static const int BG_BUFFER_SIZE = 4096;

/** ���� ��Ʈ*/
static const int BG_SERVER_PORT = 8800;
