#include "m_logger.h"
m_logger* m_logger::pLogger = NULL;
mutex m_logger::loggerMutex;

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

void m_logger::info(string logInfo){
	saveLog("[+] ", logInfo);
}

void m_logger::saveLog(string tip, string logInfo){
	m_logger::loggerMutex.lock();
	
	m_logger::loggerMutex.unlock();
}