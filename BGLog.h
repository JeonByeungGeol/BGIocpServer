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
	// ��ȿ�� �α����� Ȯ���մϴ�.
	bool Valid() { return m_logLevel != ELogLevel::BG_NONE; }

	// ���� ��û �α����� Ȯ���մϴ�.
	bool IsStopRequest() { if (m_logLevel == ELogLevel::BG_INFO && m_content == "STOP") return true; return false; }

	// �α� ������ ����ϴ�.
	ELogLevel GetLevel() { return m_logLevel; }

	// �α� ������ ����ϴ�.
	std::string GetContents() { return m_content; }

	// �α׸� ���Ͽ� ���ϴ�.
	void Write(std::fstream* fs) { *fs << m_content; }

private:
	ELogLevel m_logLevel;
	std::string m_content;
};

