#ifndef WINRMSCANNER_H_H
#define WINRMSCANNER_H_H
#include "public.h"
#include "m_http_api.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"

class WinrmScanner
{
public:
	WinrmScanner();
	static void check(string& ipAddr);
	static void pth(string& ipAddr);
	~WinrmScanner();
};
#endif
