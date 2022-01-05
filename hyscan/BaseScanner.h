#ifndef BASESCANNER_H_H
#define BASESCANNER_H_H
#include "public.h"
#define NTLMSSP_SIGNTURE "NTLMSSP"
/*
* for base scanner, write in 2021.12.09 18.32 @zpchcbd
*/

class BaseScanner
{
public:
	BaseScanner();
	virtual void check(string& ipAddr);
	virtual void pth(string& ipAddr);
	~BaseScanner();
};

#endif