#ifndef DUBBOSERVICEEXP_H_H
#define DUBBOSERVICEEXP_H_H
#include "m_base_exp.h"
class m_dubbo_exp :
	public m_base_exp
{
public:
	PortService portService;
public:
	m_dubbo_exp(PortService portService);
	~m_dubbo_exp();
	//////////////////////////////
	void checkServiceIsVuln();
	void checkUnauth();
};

#endif