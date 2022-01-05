#ifndef WMICSANNER_H_H
#define WMICSANNER_H_H
#include "BaseScanner.h"
#include "public.h"
#include "m_socket_api.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"
#include "m_wmi_api.h"

class WMIScanner :
	public BaseScanner
{
public:
	WMIAPI wmiApi;
public:
	WMIScanner();
	virtual void check(string& ipAddr);
	virtual void pth(string& ipAddr);
	void connect();
	~WMIScanner();
};

#endif