#include "m_base_exp.h"
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")	

m_base_exp::m_base_exp()
{
}


m_base_exp::~m_base_exp()
{
}

void m_base_exp::checkServiceIsVuln(){
	
}

DWORD m_base_exp::checkAliveReturn(string& ipAddr){
	DWORD dwRet;
	ULONG MacAddr[2] = { 0 };
	ULONG PhysAddrLen = 6;

	dwRet = SendARP(inet_addr(ipAddr.c_str()), 0, &MacAddr, &PhysAddrLen);
	if (dwRet == NO_ERROR){
		return 0;
	}

	return -1;
}