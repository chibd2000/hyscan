#pragma once
#include "m_base_exp.h"
class m_redis_exp :
	public m_base_exp
{
public:
	m_redis_exp(PortService portService);
	~m_redis_exp();
	void checkServiceIsVuln();
	void checkUnauth();
	void checkWeakpass();
};

