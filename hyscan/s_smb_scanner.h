#ifndef SMBSCANNER_H_H
#define SMBSCANNER_H_H
#define NTLMSSP_SIGNTURE "NTLMSSP"
#include "public.h"
#include "m_socket_api.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"
#include "m_wnet_api.h"
#pragma comment(lib, "mpr.lib")

class SMBScanner
{
public:
	SMBScanner();
	static void check(string& ipAddr);
	static void pth(string& ipAddr);
	~SMBScanner();
};

#endif