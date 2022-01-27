#ifndef LOGGER_H_H
#define LOGGER_H_H
#include "public.h"
class m_logger
{
private:
	m_logger();
public:
	static m_logger* pLogger;
	static mutex loggerMutex;
	static FILE* pFile;
	static m_logger* getLogger();
	~m_logger();
	void info(string logInfo);
	void debug(string logInfo);
	void lprintf(string logInfo);
	void saveLog(string tip, string logInfo);
};
#endif

