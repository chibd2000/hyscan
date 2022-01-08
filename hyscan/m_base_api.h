#ifndef MYAPI_H_H
#define MYAPI_H_H
#include "public.h"
class MYAPI
{
public:
	MYAPI();
	~MYAPI();
	string ConvertToStringSid(const unsigned char* bsid, const int len);
	string sid2user(PSID Sid, LPCTSTR lpSystemName);
	string getCurrentUserSid();
};
#endif