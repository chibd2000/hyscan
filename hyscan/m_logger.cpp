#include "m_logger.h"
#define LOG_FILE "./hyscan.log"
mutex m_logger::loggerMutex;
m_logger* m_logger::pLogger = m_logger::getLogger();

m_logger::m_logger()
{
}

m_logger::~m_logger()
{
}

m_logger* m_logger::getLogger(){
	if (m_logger::pLogger == NULL){
		m_logger::pLogger = new m_logger();
		return m_logger::pLogger;
	}
	return m_logger::pLogger;
}

void m_logger::debug(string logInfo){
	saveLog("[DEBUG] ", logInfo);
}

void m_logger::info(string logInfo){
	saveLog("[INFO] ", logInfo);
}

void m_logger::lprintf(string logInfo){
	printf("%s\n",logInfo.data());
}

void m_logger::saveLog(string tip, string logInfo){
	TCHAR szBufferInfo[MAX_PATH] = { 0 };
	TCHAR szTimeBuffer[64] = { 0 };
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf(szTimeBuffer, "%4d-%02d-%02d %02d:%02d:%02d ", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	sprintf(szBufferInfo, "%s [%s] %s\n", tip.data(), szTimeBuffer, logInfo.data());
	m_logger::loggerMutex.lock();
	FILE* pFile = fopen(LOG_FILE, "a");
	fprintf(pFile, szBufferInfo);
	fclose(pFile);
	m_logger::loggerMutex.unlock();
}