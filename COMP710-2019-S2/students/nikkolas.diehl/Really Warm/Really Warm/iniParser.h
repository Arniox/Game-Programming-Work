#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

class IniParser
{
public:
	IniParser();
	~IniParser();

	//Settings file
	const string iniSettingsFile;
	//Input
	ifstream myFile;
	//Settings map
	map<string, string> settingsMap;

	//Public funtionality
	string GetValueAsString(const string& iniSection, const string& key);
	int GetValueAsInt(const string iniSection, const string& key);
	float GetValueAsFloat(const string& iniSection, const string& key);
	bool GetValueAsBoolean(const string& iniSection, const string& key);

protected:
	//Protected functionality
	bool LoadIniFile(const string& filename);
	void ProcessSettingsOutput(const string& line, string& currentSection, string& variableName, string& key, string& value);

	//Key checking
	vector<string> m_keyList;
	vector<string>::iterator keyIterator;
	bool CheckExists(string* passIn);

public:
	//Conversion variables
};

