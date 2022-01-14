#ifndef LDAPSCANNER_H_H
#define LDAPSCANNER_H_H
#include "public.h"
#include "m_ldap_api.h"
class LdapScanner
{
public:
	LdapScanner();
	~LdapScanner();
	static void searchConstrainedDelegation();
	static void searchUnconstrainedDelegation();
	static void searchResourceBasedConstrainedDelegation();
	static void searchGPO();
	static void searchDomainFileServer();
	static void searchDnsRecord();
	static void searchTrustDomain();
	static void updateResourceBasedConstrainedDelegation(string pcName);
	static void updatePrivilege();
};
#endif
