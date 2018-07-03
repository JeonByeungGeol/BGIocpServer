#pragma once

#include "BGLogEnum.h"

class BGLog
{
public:
	BGLog() : m_logLevel(ELogLevel::BG_NONE) {};
	BGLog(ELogLevel level, std::string content)
		: m_logLevel(level), m_content(content)
	{}
	~BGLog();


public:
	// 유효한 로그인지 확인합니다.
	bool Valid() { return m_logLevel != ELogLevel::BG_NONE; }

	// 종료 요청 로그인지 확인합니다.
	bool IsStopRequest() { if (m_logLevel == ELogLevel::BG_INFO && m_content == "STOP") return true; return false; }

	// 로그 레벨을 얻습니다.
	ELogLevel GetLevel() { return m_logLevel; }

	// 로그 내용을 얻습니다.
	std::string GetContents() { return m_content; }

	// 로그를 파일에 씁니다.
	void Write(std::fstream* fs) { *fs << m_content; }

private:
	ELogLevel m_logLevel;
	std::string m_content;
};

