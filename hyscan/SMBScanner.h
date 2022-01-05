#ifndef SMBSCANNER_H_H
#define SMBSCANNER_H_H
#include "BaseScanner.h"
#include "public.h"
#include "m_socket_api.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"
#pragma comment(lib, "mpr.lib")

class SMBScanner :
	public BaseScanner
{
public:
	SMBScanner();
	virtual void check(string& ipAddr);
	virtual void pth(string& ipAddr);
	~SMBScanner();
};

enum IPC_ERROR{
	IPC_SUCCESS = 0x00,
	IPC_PRIVILEGE_ERROR = 0x05,
	IPC_NETWORK_ERROR = 0x43,
	LDAP_USER_PASS_ERROR = 0x52E,
};

#endif