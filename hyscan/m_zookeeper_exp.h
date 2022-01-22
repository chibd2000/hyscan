#ifndef ZOOKEEPERSERVICEEXP_H_H
#define ZOOKEEPERSERVICEEXP_H_H
#include "m_base_exp.h"
class m_zookeeper_exp :
	public m_base_exp
{
public:
	PortService portService;
public:
	m_zookeeper_exp(PortService portService);
	~m_zookeeper_exp();
	//////////////////////////////

	void m_zookeeper_exp::checkServiceIsVuln();
	void m_zookeeper_exp::checkUnauth();
};
#endif

