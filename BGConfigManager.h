#pragma once

/**
 * <pre>
 * 설정 값들을 관리
 * </pre>
*/

class BGConfigManager
{
	using Key = std::string;
	using Value = std::string;
	using ConfigMap = std::map<Key, Value>;
	
public:
	BGConfigManager();
	~BGConfigManager();

	// config.ini 파일을 읽습니다.
	bool Load();


	// config 값을 읽어 옵니다.
	std::string GetString(Key);
	int GetInt(Key);
	double GetDouble(Key);

private:
	// config 값들을 저장합니다.
	ConfigMap m_ConfigMap;
};