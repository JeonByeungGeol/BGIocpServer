#pragma once

#include "BGLog.h"

#define BG_LOG_TRACE(...)		g_LogManager.PushLog(ELogLevel::BG_TRACE,	__FUNCTION__, ##__VA_ARGS__)
#define BG_LOG_DEBUG(...)		g_LogManager.PushLog(ELogLevel::BG_DEBUG,	__FUNCTION__, ##__VA_ARGS__)
#define BG_LOG_INFO(...)		g_LogManager.PushLog(ELogLevel::BG_INFO,	__FUNCTION__, ##__VA_ARGS__)
#define BG_LOG_WARNING(...)		g_LogManager.PushLog(ELogLevel::BG_WARNING,	__FUNCTION__, ##__VA_ARGS__)
#define BG_LOG_ERROR(...)		g_LogManager.PushLog(ELogLevel::BG_ERROR,	__FUNCTION__, ##__VA_ARGS__)
#define BG_LOG_FATAL(...)		g_LogManager.PushLog(ELogLevel::BG_FATAL,	__FUNCTION__, ##__VA_ARGS__)


/**
 * <pre>
 * �α׸� �����Ѵ�.
 *
 * ���
 * 1. �α׸� ���� ���� �����Ѵ�. (trace, debug, info, err, fatal...)
 * 2. �α������� Ŀ���� �ʵ��� �Ⱓ���� ���ο� ����/������ ���⵵�� �Ѵ�.
 * 3. ���� ����� �α׸� ���� ���� �Ѵ�. (extract1, extract2, ...)
 *
 * ! ���ο� �α� Ÿ�� �߰� �� (������ �����)
 * BGLogEnum�� �α׷��� �߰� �Ŀ� Init���� �������ָ� �ȴ�.
 *
 * </pre>
*/

class BGLogManager
{
	using LogQueue = std::queue<BGLog>;

	using LogLevelLogNameMap = std::map<ELogLevel, std::string>;
	using ForderNameFileStreamMap = std::map<std::string, std::fstream*>;
	using LogLevelForderNameMap = std::map<ELogLevel, std::string>;
	
public:
	BGLogManager();
	~BGLogManager();

public:
	// �α� �ý��ۿ� �ʿ��� ������ �մϴ�.
	bool Init();

	// �α� �ý����� �����մϴ�.
	bool Start();

	// �α� �ý����� �����մϴ�.
	bool Stop();
	
	// �α� �ý��� �����û�α����� Ȯ���մϴ�.
	bool IsStopRequest(BGLog&);

	// �α� ������ �˻��մϴ�.
	bool CheckLogLevel(BGLog&);
	bool CheckLogLevel(ELogLevel);

	// �α� ������ �⺻�������� �˻��մϴ�.(�⺻�α׷��� : TRACE, DEBUG, INFO, WANING, ERROR, FATAL)
	bool IsBasicLogLevel(BGLog&);

	// queue���� �α׸� �ϳ� �����ϴ�.
	BGLog Pick();

	// queue�� �α׸� �ֽ��ϴ�.
	void PushLog(ELogLevel level, char* func_name, char* msg, ...);

	// log�� ����մϴ�.
	void Write(BGLog&);
	
	// �����̸��� �ֽ��� �ƴ϶�� ��ü�մϴ�.
	bool CheckLogFileNameAndRenew();

	// �ֽ� �����̸����� ���Ͻ�Ʈ���� �����մϴ�.
	void RenewLogFileStream();

	///////////////////////////////
	// �α� ���� ������ �Լ� �Դϴ�
	///////////////////////////////
	static void Run(BGLogManager*);

private:
	std::mutex m_queueLock;
	LogQueue m_queue;

	std::thread* m_pRunThread;
	
	struct tm m_lastCreateFileName;

	std::vector<std::pair<std::string, std::fstream*>> m_logFileStreamVec;

	LogLevelLogNameMap m_logLevelLogNameMap;
	ForderNameFileStreamMap m_forderNameFileStreamMap;
	LogLevelForderNameMap m_logLevelForderNameMap;
};

