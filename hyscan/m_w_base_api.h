#ifndef WBASEAPI_H_H
#define WBASEAPI_H_H
#include "m_base_api.h"
class m_w_base_api :
	public MYAPI
{
public:
	m_w_base_api();
	~m_w_base_api();
	string ConvertToStringSid(const unsigned char* bsid, const int len);
	string sid2user(PSID pSid, LPCTSTR lpSystemName);
	string getCurrentUserSid(string& doaminName);
};
#endif
