#ifndef OXIDSCANNER_H_H
#define OXIDSCANNER_H_H
#include "BaseScanner.h"
#include "public.h"
#include "m_socket_api.h"

class OXIDScanner :
	public BaseScanner
{
public:
	OXIDScanner();
	virtual void check(string& ipAddr);
	~OXIDScanner();
};
#endif