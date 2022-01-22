#ifndef NETSCANNER_H_H
#define NETSCANNER_H_H
#include "public.h"
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")	
class s_net_scanner
{
public:
	s_net_scanner();
	~s_net_scanner();
	static void checkAlive(string& ipAddr);
	static DWORD checkAliveReturn(string& ipAddr);
};


#endif