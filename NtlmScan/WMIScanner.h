#ifndef WMICSANNER_H_H
#define WMICSANNER_H_H
#include "BaseScanner.h"
#include "public.h"
#include "TcpClient.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"
class WMIScanner :
	public BaseScanner
{
public:
	WMIScanner();
	virtual void scan(string ipAddr);
	~WMIScanner();
};

#endif