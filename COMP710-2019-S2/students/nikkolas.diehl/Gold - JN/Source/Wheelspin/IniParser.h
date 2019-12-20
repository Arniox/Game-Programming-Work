#pragma once
#include <string>
#include <map>

class IniParser
{
public:
	IniParser();
	~IniParser();
	bool LoadIniFile(const std::string& filename);
	std::string GetValueAsString(const std::string& iniSection, const std::string& key);
	int GetValueAsInt(const std::string& iniSection, const std::string& key);
	float GetValueAsFloat(const std::string& iniSection, const std::string& key);
	double GetValueAsDouble(const std::string& iniSection, const std::string& key);
	bool GetValueAsBoolean(const std::string& iniSection, const std::string& key);
	unsigned char GetValueAsChar(const std::string& iniSection, const std::string& key);

	

protected:
	std::map<std::string, std::string> m_settingsMap;
	std::map<std::string, std::string>::iterator m_settingsMapIter;

	void RemoveSpaces(std::string & str);

	void Log(std::string str);
};

