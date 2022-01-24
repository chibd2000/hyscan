#include "s_ldap_scanner.h"


LdapScanner::LdapScanner()
{
}


LdapScanner::~LdapScanner()
{

}

void LdapScanner::searchDelegation(string serverName, string domainName)
{
	LDAP_API ldapApi(serverName, domainName);
	cout << "---------------------searchConstrainedDelegation-----------------------------" << endl;
	ldapApi.searchConstrainedDelegation();
	cout << "---------------------searchUnconstrainedDelegation---------------------------" << endl;
	ldapApi.searchUnconstrainedDelegation();
	cout << "---------------------searchResourceBasedConstrainedDelegation----------------" << endl;
	ldapApi.searchResourceBasedConstrainedDelegation();
}

void LdapScanner::searchGPO(string serverName, string domainName){
	LDAP_API ldapApi(serverName, domainName);
	ldapApi.searchGPO();
}

void LdapScanner::addDnsRecord(string serverName, string domainName, string dnsName, string ipName){
	LDAP_API ldapApi(serverName, domainName);
	ldapApi.addDnsRecord(dnsName, ipName);
}


void LdapScanner::addComputer(string serverName, string domainName, string pcName){
	LDAP_API ldapApi(serverName, domainName);
	ldapApi.addComputer(pcName);
}

void LdapScanner::searchDomainFileServer(string serverName, string domainName){
	LDAP_API ldapApi(serverName, domainName);
	ldapApi.searchDomainFileServer();

}

void LdapScanner::searchDnsRecord(string serverName, string domainName){
	LDAP_API ldapApi(serverName, domainName);
	ldapApi.searchDnsRecord();
}

void LdapScanner::searchTrustDomain(string serverName, string domainName){
	LDAP_API ldapApi(serverName, domainName);
	ldapApi.searchTrustDomain();
}

void LdapScanner::updateResourceBasedConstrainedDelegation(string serverName, string domainName, string pcName){
	LDAP_API ldapApi(serverName, domainName);
	ldapApi.updateResourceBasedConstrainedDelegation(pcName);

}

void LdapScanner::updatePrivilege(string serverName, string domainName){
	LDAP_API ldapApi(serverName, domainName);
	ldapApi.updatePrivilege();
}

void LdapScanner::searchLAPs(string serverName, string domainName){
	LDAP_API ldapApi(serverName, domainName);
	ldapApi.searchLAPs();
}