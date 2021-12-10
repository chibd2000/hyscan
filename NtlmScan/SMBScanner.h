#ifndef SMBSCANNER_H_H
#define SMBSCANNER_H_H
#include "BaseScanner.h"

class SMBScanner :
	public BaseScanner
{
public:
	SMBScanner();
	virtual void scan(string ipAddr);
	~SMBScanner();
};

#endif