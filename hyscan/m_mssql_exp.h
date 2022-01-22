#pragma once
#include "m_base_exp.h"
class m_mssql_exp :
	public m_base_exp
{
public:
	m_mssql_exp(PortService portService);
	~m_mssql_exp();
	void m_mssql_exp::checkServiceIsVuln();
	void m_mssql_exp::checkWeakpass();
};

