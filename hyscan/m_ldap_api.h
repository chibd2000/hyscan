#ifndef LDAPAPI_H_H
#define LDAPAPI_H_H
#include "m_base_api.h"
#include "Tools.h"
#include <Winldap.h>
#include <sddl.h>
#pragma comment(lib,"Wldap32.lib")
/*
write in 2021.12.14 13.00 for domain ldap @zpchcbd @ske¥Û ¶–÷ *.*
*/
class LDAP_API : public MYAPI
{
private:
	string username;
	string password;
	string ldapServerAddr;
	string domainName;
	LDAP_TIMEVAL defaultTimeVal;
	LDAPMessage* pLdapMessage;
	LDAPMessage* pEntry;
	LDAP* pLdapInstance;
public:
	LDAP_API(string ldapServerAddr, string domainName);
	LDAP_API(string username, string password, string ldapServerAddr, string domainName);
	~LDAP_API();
	void init();
	DWORD setLdapOption();
	DWORD connect();
	DWORD bind();
	void search(string filterGrammar, PCHAR searchAttabuite[]);
	void searchConstrainedDelegation();
	void searchUnconstrainedDelegation();
	void searchResourceBasedConstrainedDelegation();
	void closeLdapConnection();
	SEC_WINNT_AUTH_IDENTITY_W initLdapCreds();
};

#endif