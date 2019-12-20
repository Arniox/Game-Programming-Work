//Locals
#include "IniParser.h"
#include "logmanager.h"

//Libraries
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <locale>

IniParser::IniParser()
{
}
IniParser::~IniParser()
{
}

bool
IniParser::LoadIniFile(const std::string & filename)
{
	std::ifstream iniFile(filename);

	if (iniFile.is_open())
	{
		std::string sectionHead = "";
		std::string line;

		while (std::getline(iniFile, line))
		{
			if (line.size() > 0)
			{
				// Check if section header
				if (line[0] == '[')
				{
					line.erase(0, 1);
					line.erase(line.size() - 1, 1);
					sectionHead = line;
				}
				else if (line.find('=') && line[0] != '#') // Check if key + value, excluding comment lines
				{
					// Extract the key from the line, splitting at the first occurance of '='
					std::string key = line.substr(0, line.find('='));
					// Remove any spaces from the key
					RemoveSpaces(key);

					// Extract the values string from the line, removing the '=' delimiter
					std::string value = line.substr(line.find('=') + 1, line.size());

					// Check if the value is a string (text encased by "")
					if (value.find('\"') == std::string::npos)
					{
						RemoveSpaces(value);
					}
					else
					{
						// Remove any leading spaces from the title
						while (value[0] == ' ')
						{
							value.erase(0, 1);
						}
							
						// Remove extra quotation marks from string
						std::string::iterator end = std::remove(value.begin(), value.end(), '\"');
						value.erase(end, value.end());
					}

					// Create a key name
					std::string keyName = sectionHead + "|" + key;

					// Create and add the key pair to the map
					m_settingsMap.emplace(keyName, value);
				}
			}
		}

		if (iniFile.is_open())
			iniFile.close();

		return true;
	}
	
	return false;

	
}

std::string
IniParser::GetValueAsString(const std::string& iniSection, const std::string & key)
{
	std::string value = "";

	m_settingsMapIter = m_settingsMap.find(iniSection + "|" + key);

	if (m_settingsMapIter != m_settingsMap.end())
	{
		value = m_settingsMapIter->second;

		Log("SUCCESS - Parser found string in map matching key: " + iniSection + "|" + key + " of: " + value);
		return value;
	}
	else 
	{
		Log("ERROR - Parser could not find string in map matching key: " + iniSection + "|" + key);
		return "ERROR";
	}
}

int
IniParser::GetValueAsInt(const std::string & iniSection, const std::string & key)
{
	int value = 0;

	m_settingsMapIter = m_settingsMap.find(iniSection + "|" + key);

	if (m_settingsMapIter != m_settingsMap.end())
	{
		value = std::stoi(m_settingsMapIter->second);

		Log("SUCCESS - Parser found integer in map matching key: " + iniSection + "|" + key + " of: " + std::to_string(value));
		return value;
	}
	else 
	{
		Log("ERROR - Parser could not find integer in map matching key: " + iniSection + "|" + key);
		return -1;
	}
}

float
IniParser::GetValueAsFloat(const std::string & iniSection, const std::string & key)
{
	float value = 0.0f;

	m_settingsMapIter = m_settingsMap.find(iniSection + "|" + key);

	if (m_settingsMapIter != m_settingsMap.end())
	{
		value = std::stof(m_settingsMapIter->second);

		Log("SUCCESS - Parser found floating point number in map matching key: " + iniSection + "|" + key + " of: " + std::to_string(value));
		return value;
	}
	else {
		
		Log("ERROR - Parser could not find floating point number in map matching key: " + iniSection + "|" + key);
		return -1.0f;
	}

}

double 
IniParser::GetValueAsDouble(const std::string & iniSection, const std::string & key)
{
	double value = 0.0;

	m_settingsMapIter = m_settingsMap.find(iniSection + "|" + key);

	if (m_settingsMapIter != m_settingsMap.end())
	{
		value = std::stod(m_settingsMapIter->second);

		Log("SUCCESS - Parser found double in map matching key: " + iniSection + "|" + key + " of: " + std::to_string(value));
		return value;
	}
	else {
		
		Log("ERROR - Parser could not find double in map matching key: " + iniSection + "|" + key);
		return -1.0;
	}
}

bool
IniParser::GetValueAsBoolean(const std::string & iniSection, const std::string & key)
{
	bool value = false;

	m_settingsMapIter = m_settingsMap.find(iniSection + "|" + key);

	if (m_settingsMapIter != m_settingsMap.end())
	{
		std::string keyValue = m_settingsMapIter->second;
		if (keyValue == "true" || keyValue == "yes") {
			value = true;
		}
		else {
			value = false;
		}

		Log("SUCCESS - Parser found boolean in map matching key: " + iniSection + "|" + key + " of: " + (value ? "TRUE" : "FALSE"));
		return value;
	}
	else {
		
		Log("ERROR - Parser could not find boolean in map matching key: " + iniSection + "|" + key);
		return false;
	}
}

unsigned char 
IniParser::GetValueAsChar(const std::string & iniSection, const std::string & key)
{
	unsigned char value = 0;
	
	m_settingsMapIter = m_settingsMap.find(iniSection + "|" + key);

	if (m_settingsMapIter != m_settingsMap.end()) {

		int intValue = std::stoi(m_settingsMapIter->second);
		value = static_cast<unsigned char>(intValue);

		Log("SUCCESS - Parser found integer in map matching key: " + iniSection + "|" + key + " of: " + std::to_string(value));
		return value;
	}
	else {
		
		Log("ERROR - Parser could not find char in map matching key: " + iniSection + "|" + key);
		return '?';
	}
}

void
IniParser::RemoveSpaces(std::string& str)
{
	std::string::iterator end = std::remove(str.begin(), str.end(), ' ');
	str.erase(end, str.end());
}

void
IniParser::Log(std::string str)
{
	LogManager::Debug(str.c_str());
	LogManager::GetInstance().DestroyInstance();
}
