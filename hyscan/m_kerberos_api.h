#ifndef KERBEROS_H_H
#define KERBEROS_H_H
#include "public.h"
class m_kerberos_api
{
private:
	string username;
	string password;
public:
	m_kerberos_api();
	~m_kerberos_api();
	void getTgt();
	void getST();
	void getTgtTicket();
	void getStTicket();
};
#endif 

