// COMP710 GP 2D Framework 2019

// This include:
#include "logmanager.h"
#include "game.h"
#include "IniParser.h"

// Library includes:
#include <Windows.h>

// Static Members:
LogManager* LogManager::sm_pInstance = 0;
bool LogManager::m_debugLog = false;

LogManager& 
LogManager::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new LogManager();
	}

	return (*sm_pInstance);
}

LogManager::LogManager()
{
}

LogManager::~LogManager()
{

}

void
LogManager::Write(const char* pcMessage)
{
	LogManager::GetInstance().Log(pcMessage);
}

void 
LogManager::Title(const char * pcMessage)
{
	Write("------------");
	Write(pcMessage);
	Write("------------");
}

void
LogManager::Debug(const char* pcMessage)
{
	LogManager& lm = LogManager::GetInstance();
	if (m_debugLog)
		lm.Log(pcMessage);
}

void 
LogManager::Log(const char* pcMessage)
{
	OutputDebugStringA(pcMessage);
	OutputDebugStringA("\n"); 
}

void
LogManager::SetDebugLogging(const bool value)
{
	m_debugLog = value;
}

void
LogManager::DestroyInstance() {
	delete sm_pInstance;
	sm_pInstance = nullptr;
}