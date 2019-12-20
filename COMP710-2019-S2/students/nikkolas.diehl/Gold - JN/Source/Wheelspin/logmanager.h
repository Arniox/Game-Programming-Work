// COMP710 GP 2D Framework 2019
#ifndef __LOGMANAGER_H__
#define __LOGMANAGER_H__

class LogManager
{
	//Member Methods:
public:
	static LogManager& GetInstance();
	~LogManager();

	static void Write(const char* pcMessage);
	static void Title(const char* pcMessage);
	static void Debug(const char* pcMessage);
	void Log(const char* pcMessage);
	void SetDebugLogging(const bool value);
protected:

private:
	LogManager();
	LogManager(const LogManager& logManager);
	LogManager& operator=(const LogManager& logManager);

	//Member Data:
public:
	void DestroyInstance();

protected:
	static LogManager* sm_pInstance;
	static bool m_debugLog;

private:
	

};

#endif //__LOGMANAGER_H__
