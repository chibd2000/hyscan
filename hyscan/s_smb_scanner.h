#ifndef SMBSCANNER_H_H
#define SMBSCANNER_H_H
#define NTLMSSP_SIGNTURE "NTLMSSP"
#include "public.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"
///////////////////////
#include "m_socket_api.h"
#include "m_wnet_api.h"
///////////////////////
#include "s_net_scanner.h"
//////////////////////////
#pragma comment(lib, "mpr.lib")

class SMBScanner
{
public:
	SMBScanner();
	static void check(string& ipAddr);
	static void pth(string& ipAddr);
	static void pth2(string& ipAddr, string& username, string& password);
	~SMBScanner();
};

#endif