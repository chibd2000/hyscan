#ifndef WEAKSCANNER_H_H
#define WEAKSCANNER_H_H
#include "public.h"
#include "m_wnet_api.h"
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")
class WeakScanner
{
public:
	WeakScanner();
	~WeakScanner();
	static void check(string& serverName, string& domainName, string& domainUsername, string& domainPassowrd, string& personalPassword);
};
#endif

