#ifndef NETSCANNER_H_H
#define NETSCANNER_H_H
#include "public.h"
#include "m_wnet_api.h"
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")	
class s_net_scanner
{
public:
	s_net_scanner();
	~s_net_scanner();
	static void checkAlive(string& ipAddr);
	static DWORD checkAliveReturn(string& ipAddr);
	static void getNetSession(string& ipAddr);
	static void getNetShare(string& ipAddr);
	static void getLoggedUsers(string& ipAddr);
};


#endif