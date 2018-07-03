#pragma once

/**
 * <pre>
 * ���� ������ ����
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

	// config.ini ������ �н��ϴ�.
	bool Load();


	// config ���� �о� �ɴϴ�.
	std::string GetString(Key);
	int GetInt(Key);
	double GetDouble(Key);

private:
	// config ������ �����մϴ�.
	ConfigMap m_ConfigMap;
};