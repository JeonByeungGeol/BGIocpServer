#include "stdafx.h"
#include "BGConfigManager.h"



BGConfigManager::BGConfigManager()
{
}


BGConfigManager::~BGConfigManager()
{
	m_ConfigMap.clear();
}

bool BGConfigManager::Load()
{
	std::fstream fs;
	fs.open(BG_CONFIG_FILE_NAME, std::ios::in);

	if (!fs.is_open())
		return false;

	Key k;
	Value v;

	while (fs >> k) {
		if (k == "//") {
			std::getline(fs, v);
			continue;
		}

		fs >> v;

		m_ConfigMap.insert(std::make_pair(k, v));
	}

	return true;
}

std::string BGConfigManager::GetString(Key k)
{
	auto iter = m_ConfigMap.find(k);
	if (iter == m_ConfigMap.end()) {
		return std::string();
	}
		
	return iter->second;
}

int BGConfigManager::GetInt(Key k)
{
	Value v = GetString(k);
	if (v.empty())
		return 0;
	
	return std::stoi(v);	
}

double BGConfigManager::GetDouble(Key k)
{
	Value v = GetString(k);
	if (v.empty())
		return 0.0;
		
	return std::stod(v);
}