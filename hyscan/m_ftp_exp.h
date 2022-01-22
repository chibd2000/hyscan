#ifndef FTPSERVICEEXP_H_H
#define FTPSERVICEEXP_H_H
#include "m_base_exp.h"

class m_ftp_exp :public m_base_exp{
public:
	PortService portService;
public:
	m_ftp_exp(PortService portService);
	~m_ftp_exp();
	//////////////////////////////
	void checkServiceIsVuln();
	void checkWeakpass();
};
#endif
