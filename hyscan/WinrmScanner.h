#ifndef WINRMSCANNER_H_H
#define WINRMSCANNER_H_H
#include "BaseScanner.h"
#include "public.h"
#include "m_http_api.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"

class WinrmScanner :
	public BaseScanner
{
public:
	WinrmScanner();
	virtual void check(string& ipAddr);
	virtual void pth(string& ipAddr);
	~WinrmScanner();
};
#endif
