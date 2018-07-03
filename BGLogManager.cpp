#include "stdafx.h"
#include "BGLogManager.h"


BGLogManager::BGLogManager()
{
}


BGLogManager::~BGLogManager()
{
}

/**
 * !새로운 로그를 추가하면, 이곳에 추가해야 합니다.
 * 로그 시스템에 필요한 설정을 합니다.
 * logLevel 이 어떤 로그파일에 씌어야 하는지 설정합니다.
 * 필요한 파일 스트림을 생성합니다.
*/
bool BGLogManager::Init()
{
	// 마지막 로그파일 생성 시간
	m_lastCreateFileName.tm_hour = -1;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 로그레벨 - 로그이름 매칭
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_NONE, "NONE"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_TRACE, "TRACE"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_DEBUG, "DEBUG"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_INFO, "INFO"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_WARNING, "WARNING"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_ERROR, "ERROR"));
	m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_FATAL, "FATAL"));	
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 폴더이름 - 파일스트림 매칭
	std::fstream* pFileStream{ nullptr };
	
	// default 로그 파일
	pFileStream = new std::fstream;
	m_logFileStreamVec.push_back(std::make_pair("log\\", pFileStream));
	m_forderNameFileStreamMap.insert(std::make_pair("log", pFileStream));	

	pFileStream = new std::fstream;
	m_logFileStreamVec.push_back(std::make_pair("err\\", pFileStream));
	m_forderNameFileStreamMap.insert(std::make_pair("err", pFileStream));	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 로그레벨 - 폴더이름 매칭 (매칭 안되있는 건 default폴더)
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_NONE, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_WARNING, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_ERROR, "err"));
	m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_FATAL, "err"));	
	/////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * 새로운 로그 타입 추가시
	 * 1번
	 * m_logLevelLogNameMap.insert(std::make_pair(ELogLevel::BG_EXTRACT_DATA_1, "EXTRACT_DATA_1"));
	 * 2번
	 * pFileStream = new std::fstream;
	 * m_logFileStreamVec.push_back(std::make_pair("extract_data_1", pFileStream));
	 * m_forderNameFileStreamMap.insert(std::make_pair("extract_data_1", pFileStream));
	 * 3번
	 * m_logLevelForderNameMap.insert(std::make_pair(ELogLevel::BG_EXTRACT_DATA_1, "extract_data_1"));
	*/
	return true;
}

/**
 * 이 함수를 호출하면 로그시스템이 시작합니다.
 * 로그를 찍는 전용 스레드를 만들어 동작
*/
bool BGLogManager::Start()
{
	if (!Init())
		return false;
	
	if (m_pRunThread != nullptr)
		return false;

	m_pRunThread = new std::thread{BGLogManager::Run, this};
	
	return true;
}

/**
 * 이 함수를 호출하면 로그시스템이 종료합니다.
 * 로그 시스템 종료 요청 로그를 전용스레드에 보낸후에
 * 로그전용 스레드가 확인하면, 스레드가 종료된 것을 확인한 뒤에
 * true를 리턴합니다.
*/
bool BGLogManager::Stop()
{
	m_queueLock.lock();
	m_queue.push(BGLog{ELogLevel::BG_INFO, "STOP"});
	m_queueLock.unlock();

	m_pRunThread->join();

	delete m_pRunThread;
	m_pRunThread = nullptr;
	
	for (auto fStream : m_logFileStreamVec) {
		if (fStream.second->is_open())
			fStream.second->close();
		delete fStream.second;
		fStream.second = nullptr;
	}

	return true;
}

/**
 * 로그 시스템이 동작종료를 요청하는 로그가 들어왔는지 확인합니다.
 * 종료 요청 로그는 [로그레벨 : INFO, 내용 : "STOP"] 입니다.
 * 로그 전용 스레드에서만 호출 합니다.
 * true를 리턴하면, 로그 전용 스레드는 종료됩니다.
*/
bool BGLogManager::IsStopRequest(BGLog& log)
{
	return log.IsStopRequest();
}

/**
 * 기록해야 하는 로그인지 로그레벨로 검사합니다.
 * bg_config.ini파일에 LogLevel의 값보다 커야
 * 실제 로그를 기록할 수 있습니다.
*/
bool BGLogManager::CheckLogLevel(BGLog & log)
{
	return (static_cast<int>(log.GetLevel()) > g_Config.GetInt("LogLevel"));
}

bool BGLogManager::CheckLogLevel(ELogLevel level)
{
	return (static_cast<int>(level) > g_Config.GetInt("LogLevel"));
}

/**
 * 로그레벨이 기본로그인지 검사합니다.
 * 기본로그는 TRACE, DEBUG, INFO, WANING, ERROR, FATAL
 * 6가지가 존재합니다.
*/
bool BGLogManager::IsBasicLogLevel(BGLog &log)
{
	auto level = log.GetLevel();
	switch(level) {
		case ELogLevel::BG_TRACE:
		case ELogLevel::BG_DEBUG:
		case ELogLevel::BG_INFO:
		case ELogLevel::BG_WARNING:
		case ELogLevel::BG_ERROR:
		case ELogLevel::BG_FATAL:
			return true;
		default:
			return false;
	}
}

/**
 * Queue에서 로그를 꺼냅니다.
 * 꺼낸 로그는 Valid를 호출해서
 * 유효한 로그인지 확인한 후에 사용합니다.
*/
BGLog BGLogManager::Pick()
{
	m_queueLock.lock();		//--*--*--*--*--*--*--
	
	if (m_queue.empty()) {
		m_queueLock.unlock();	//**-**-**-**-**-**
		return BGLog{};
	}

	BGLog log = m_queue.front();	
	m_queue.pop();

	m_queueLock.unlock();	//**-**-**-**-**-**

	return log;
}

/**
 * 로그를 시간, 레벨, 내용, 호출한 함수로 조합하여 Queue에 넣습니다.
 * 이후 전용스레드에서 하나씩 꺼내 처리합니다.
*/
void BGLogManager::PushLog(ELogLevel level, char* func_name, char* msg, ...)
{
	if (!CheckLogLevel(level))
		return;

	struct tm ltm;
	time_t t = time(NULL);
	localtime_s(&ltm, &t);


	// 로그 내용
	char tmp[4000] = "";	
	char result[4096] = "";
	va_list args;
	va_start(args, msg);
	vsprintf_s(tmp, _countof(tmp), msg, args);
	va_end(args);

	// 로그 이름
	std::string logName{ "[" };
	auto name = m_logLevelLogNameMap.find(level);
	if (name == m_logLevelLogNameMap.end()) {
		BG_LOG_ERROR("logLevelLogNameMap.find() Failed! [logLevel=%d]", static_cast<int>(level));
		return;
	}
	logName.append(name->second);
	logName.append("]");

	// 로그 시간
	char time_tmp[100] = "";
	sprintf_s(time_tmp, "[%02d:%02d:%02d]", ltm.tm_hour, ltm.tm_min, ltm.tm_sec);
	std::string logTime{ time_tmp };

	// 로그 발생한 함수
	std::string logFunc{ "(" };
	logFunc.append(func_name);
	logFunc.append(")");

	std::string logContents{ tmp };
	logContents.append(" - ");
	logContents.append(logFunc);

	// 조합
	sprintf_s(result, "%-10s %-7s %-s\n"
		, logTime.c_str()
		, logName.c_str()
		, logContents.c_str());

	BGLog log{ level, result };
	
	m_queueLock.lock();
	m_queue.push(log);
	m_queueLock.unlock();
}

/**
 * 로그를 알맞은 파일에 기록합니다.
 * 특정 로그파일이 없으면 디폴트 파일에 기록합니다.
 * 디폴트 파일은 log폴더에 기록하고, m_logfileStreamVec 0번째 인덱스에 존재합니다.
*/
void BGLogManager::Write(BGLog &log)
{
	if (!CheckLogFileNameAndRenew()) {
		RenewLogFileStream();
	}

	std::fstream* pDefault{ nullptr };
	if (IsBasicLogLevel(log)) {
		pDefault = m_logFileStreamVec[0].second;
	}

	std::fstream* pSpecific{ nullptr };
	auto iter1 = m_logLevelForderNameMap.find(log.GetLevel());
	if (iter1 != m_logLevelForderNameMap.end()) {
		auto iter2 = m_forderNameFileStreamMap.find(iter1->second);
		if (iter2 != m_forderNameFileStreamMap.end())
			pSpecific = iter2->second;		
	}
	
	if (pDefault) {
		log.Write(pDefault);
		
		printf(log.GetContents().c_str());
		fflush(stdout);
	}
	
	if (pSpecific)
		log.Write(pSpecific);
}

/**
 * 파일이름은 시간단위로 갱신됩니다.
 * 파일이 갱신되면 false를 리턴합니다.
 * 기존 파일을 그대로 사용할 수 있으면 true를 리턴합니다.
*/
bool BGLogManager::CheckLogFileNameAndRenew()
{
	struct tm ltm;
	time_t t = time(NULL);
	localtime_s(&ltm, &t);

	if (ltm.tm_hour != m_lastCreateFileName.tm_hour) {
		m_lastCreateFileName = ltm;
		return false;
	}

	return true;
}

/**
 * 기존 파일스트림을 닫고,
 * 최신 파일 이름에 맞게 새로운 파일스트림을 생성합니다.
 * CheckLogFileNameAndRenew()함수가 false를 리턴하면 해당함수를 호출합니다.
*/
void BGLogManager::RenewLogFileStream()
{
	char timestr[32];
	sprintf_s(timestr, "%04d-%02d-%02d_%02d.LOG", m_lastCreateFileName.tm_year + 1900,
		m_lastCreateFileName.tm_mon + 1,
		m_lastCreateFileName.tm_mday,
		m_lastCreateFileName.tm_hour);

	for (auto fileStream : m_logFileStreamVec) {

		if (fileStream.second->is_open())
			fileStream.second->close();

		std::string fileName{ fileStream.first };
		fileName.append(timestr);
		fileStream.second->open(fileName, std::ios::app);

	}
}

/**
 * 로그 전용 스레드 함수 입니다.
 * queue에서 로그를 꺼내 기록합니다.
 * 로그 종료 요청이 있을때가지 동작합니다.
 * Stop함수를 호출하면 해당 함수는 종료됩니다.
*/
void BGLogManager::Run(BGLogManager* pLogMgr)
{
	while (true)
	{		
		BGLog log = pLogMgr->Pick();
		if (!log.Valid())
			continue;
		
		if (pLogMgr->IsStopRequest(log)) {
			return;
		}
		
		// 로그 쓰기
		pLogMgr->Write(log);				
	}

	return;
}
