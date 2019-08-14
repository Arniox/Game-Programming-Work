//Libraries
#include <iostream>
#include <fstream>
#include <string>

//Locals
#include "iniParser.h"

//Namespaces
using namespace std;

IniParser::IniParser()
	:iniSettingsFile("assets/settings.ini")
	,myFile()
{
	//Individual whole line
	string line;

	//currentSection, variableName, key and value
	string currentSection;
	string variableName;
	string value;
	string key;

	if (LoadIniFile(iniSettingsFile)) {
		while (getline(myFile, line)) {
			ProcessSettingsOutput(line, currentSection, variableName, key, value);
			if (!currentSection.empty() && !variableName.empty()) {

				key = currentSection + " | " + variableName;
				//Create the map
				settingsMap[key] = (!value.empty() ? value : "null");
				//Reset strings
				value = "";
				key = "";
				variableName = "";
			}
		}
	}
}


IniParser::~IniParser()
{
	myFile.close();
}

bool 
IniParser::LoadIniFile(const string & filename)
{
	myFile.open(filename);
	return myFile.is_open();
}

void 
IniParser::ProcessSettingsOutput(const string& line, string& currentSection, string& variableName, string& key, string& value)
{
	//If line not empty
	if (!line.empty()) {
		if (line.at(0) == '[') { //Get section
			for (int i = 1; i < line.size(); ++i) {
				if (line.at(i) != ']') {
					currentSection += line.at(i);
				}
			}
		}
		else if (line.at(0) != '[') { //Get data
			int i = 0;
			//Get variable name
			while (line.at(i) != '=') {
				variableName += line.at(i);
				++i;
			}
			//If the variable passed is finished
			if (line.at(i) == '=') {
				//Remove space and then force iteration forward by 1
				variableName.erase(i - 1, 1);
				i += 2;
			}
			//Get value
			while (i < line.size()) {
				value += line.at(i);
				++i;
			}
		}
	}
	
}

string 
IniParser::GetValueAsString(const string & iniSection, const string & key)
{
	return settingsMap[iniSection + " | " + key];
}

int 
IniParser::GetValueAsInt(const string iniSection, const string & key)
{
	//Convert string to int
	return stoi(settingsMap[iniSection + " | " + key]);
}

float 
IniParser::GetValueAsFloat(const string & iniSection, const string & key)
{
	return stof(settingsMap[iniSection + " | " + key]);
}

bool 
IniParser::GetValueAsBoolean(const string & iniSection, const string & key)
{
	string value = settingsMap[iniSection + " | " + key];
	if (toupper(value.at(0)) == 'F' || toupper(value.at(0)) == 'N') {
		return false;
	}
	else if (toupper(value.at(0)) == 'T' || toupper(value.at(0)) == 'Y') {
		return true;
	}
	else {
		//return false if it's anything but true or false
		return false;
	}
}
