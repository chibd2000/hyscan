#ifndef MONGODBSERVICEEXP_H_H
#define MONGODBSERVICEEXP_H_H

#include "m_base_exp.h"
class m_mongodb_exp :
	public m_base_exp
{
public:
	PortService portService;
public:
	m_mongodb_exp(PortService portService);
	~m_mongodb_exp();
	//////////////////////////////
	void checkServiceIsVuln();
	void checkUnauth();
};

#endif