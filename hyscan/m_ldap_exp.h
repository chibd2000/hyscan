#ifndef LDAPSERVICEEXP_H_H
#define LDAPSERVICEEXP_H_H

#include "m_base_exp.h"
#include <Winldap.h>
class m_ldap_exp :
	public m_base_exp
{
public:
	m_ldap_exp(PortService portService);
	~m_ldap_exp();
	void m_ldap_exp::checkServiceIsVuln();
	void m_ldap_exp::checkUnauth();
};
#endif