#ifndef MEMCACHESERVICEEXP_H_H
#define MEMCACHERVICEEXP_H_H
#include "m_base_exp.h"
class m_memcache_exp :
	public m_base_exp
{
public:
	PortService portService;
public:
	m_memcache_exp(PortService portService);
	~m_memcache_exp();
	//////////////////////////////
	void checkServiceIsVuln();
	void checkUnauth();
};



#endif