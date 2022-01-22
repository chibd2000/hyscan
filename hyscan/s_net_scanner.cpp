#include "s_net_scanner.h"

extern vector<string> g_vAliveIp;
s_net_scanner::s_net_scanner()
{
}


s_net_scanner::~s_net_scanner()
{
}

void s_net_scanner::checkAlive(string& ipAddr){
	DWORD dwRet;
	ULONG MacAddr[2] = { 0 };
	ULONG PhysAddrLen = 6;

	dwRet = SendARP(inet_addr(ipAddr.c_str()), 0, MacAddr, &PhysAddrLen);
	if (dwRet == NO_ERROR){
		//if (PhysAddrLen == 6){
			//PBYTE pbHexMac = (PBYTE)MacAddr;
			//printf("Mac_Address: %02X %02X %02X %02X %02X %02X\n",pbHexMac[0], pbHexMac[1], pbHexMac[2], pbHexMac[3], pbHexMac[4], pbHexMac[5]);
		//}
		g_vAliveIp.push_back(ipAddr);
	}
}

DWORD s_net_scanner::checkAliveReturn(string& ipAddr){
	DWORD dwRet;
	ULONG MacAddr[2] = { 0 };
	ULONG PhysAddrLen = 6;

	dwRet = SendARP(inet_addr(ipAddr.c_str()), 0, &MacAddr, &PhysAddrLen);
	if (dwRet == NO_ERROR){
		return 0;
	}

	return -1;
}
