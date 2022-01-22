#ifndef LDAPSCANNER_H_H
#define LDAPSCANNER_H_H
#include "public.h"
#include "m_ldap_api.h"
class LdapScanner
{
public:
	LdapScanner();
	~LdapScanner();
	static void searchDelegation(string serverName, string domainName);
	static void searchGPO(string serverName, string domainName);
	static void addComputer(string serverName, string domainName, string pcName, string dn);
	static void searchDomainFileServer(string serverName, string domainName);
	static void searchDnsRecord(string serverName, string domainName);
	static void searchTrustDomain(string serverName, string domainName);
	static void updateResourceBasedConstrainedDelegation(string serverName, string domainName, string pcName);
	static void updatePrivilege(string serverName, string domainName);
	static void addDnsRecord(string serverName, string domainName, string dnsName, string ip);
	static void searchLAPs(string serverName, string domainName);
};
#endif
