#ifndef OXIDSCANNER_H_H
#define OXIDSCANNER_H_H
#include "public.h"
#include "m_socket_api.h"

class OXIDScanner
{
public:
	OXIDScanner();
	static void check(string& ipAddr);
	~OXIDScanner();
};
#endif