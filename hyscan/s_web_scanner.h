#ifndef WEBSCANNER_H_H
#define WEBSCANNER_H_H
#include "public.h"
#include "m_http_api.h"
class s_web_scanner
{
public:
	s_web_scanner();
	~s_web_scanner();
	static void scan(string& ipAddr, string& port);

};
#endif
