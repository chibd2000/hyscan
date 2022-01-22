#ifndef RMISERVICEEXP_H_H
#define RMISERVICEEXP_H_H
#include "m_base_exp.h"

class m_javarmi_exp: public m_base_exp
{
public:
	PortService portService;
public:
	m_javarmi_exp(PortService portService);
	~m_javarmi_exp();
	void checkServiceIsVuln();
	void checkDeserialization();
};

#endif
