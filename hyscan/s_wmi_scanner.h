#ifndef WMICSANNER_H_H
#define WMICSANNER_H_H
#include "public.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"
//////////////////////
#include "m_socket_api.h"
#include "m_wmi_api.h"
/////////////////////
#include "s_net_scanner.h"
class WMIScanner //:public BaseScanner
{
	
public:
	WMIScanner();
	static void check(string& ipAddr);
	static void pth(string& ipAddr);
	~WMIScanner();
};

#endif