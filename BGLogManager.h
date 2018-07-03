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
 * 로그를 관리한다.
 *
 * 기능
 * 1. 로그를 레벨 별로 관리한다. (trace, debug, info, err, fatal...)
 * 2. 로그파일이 커지지 않도록 기간마다 새로운 폴더/파일이 생기도록 한다.
 * 3. 정보 추출용 로그를 따로 관리 한다. (extract1, extract2, ...)
 *
 * ! 새로운 로그 타입 추가 시 (데이터 추출용)
 * BGLogEnum에 로그레벨 추가 후에 Init에서 셋팅해주면 된다.
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
	// 로그 시스템에 필요한 설정을 합니다.
	bool Init();

	// 로그 시스템을 시작합니다.
	bool Start();

	// 로그 시스템을 종료합니다.
	bool Stop();
	
	// 로그 시스템 종료요청로그인지 확인합니다.
	bool IsStopRequest(BGLog&);

	// 로그 레벨을 검사합니다.
	bool CheckLogLevel(BGLog&);
	bool CheckLogLevel(ELogLevel);

	// 로그 레벨이 기본레벨인지 검사합니다.(기본로그레벨 : TRACE, DEBUG, INFO, WANING, ERROR, FATAL)
	bool IsBasicLogLevel(BGLog&);

	// queue에서 로그를 하나 꺼냅니다.
	BGLog Pick();

	// queue에 로그를 넣습니다.
	void PushLog(ELogLevel level, char* func_name, char* msg, ...);

	// log를 기록합니다.
	void Write(BGLog&);
	
	// 파일이름이 최신이 아니라면 교체합니다.
	bool CheckLogFileNameAndRenew();

	// 최신 파일이름으로 파일스트림을 생성합니다.
	void RenewLogFileStream();

	///////////////////////////////
	// 로그 전용 스레드 함수 입니다
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

