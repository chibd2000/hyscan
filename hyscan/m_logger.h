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
	static m_logger* getLogger();
	~m_logger();
	void debug();
	void info();
	void saveLog();
};
#endif

