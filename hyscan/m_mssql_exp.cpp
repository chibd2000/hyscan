#include "m_mssql_exp.h"


m_mssql_exp::m_mssql_exp(PortService portService)
{
	this->portService = portService;
}


m_mssql_exp::~m_mssql_exp()
{
}
void m_mssql_exp::checkServiceIsVuln(){
	if (!checkAliveReturn(this->portService.serviceIpAddr)){
		this->checkWeakpass();
	}
}

void m_mssql_exp::checkWeakpass(){
	


}