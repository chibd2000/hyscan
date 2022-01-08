#ifndef LDAPAPI_H_H
#define LDAPAPI_H_H
#include "m_base_api.h"
#include "Tools.h"
#include <Winldap.h>
#include <Iads.h>
#include <adshlp.h>
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Activeds.lib")
#pragma comment(lib, "adsiid.lib")
/*
write in 2021.12.14 13.00 for domain ldap @zpchcbd @ske¥Û ¶–÷ *.*
*/
class MACHINE_ACCOUNT_ATTRIBUTE{
public:
	string dnsHostName;
	string samAccountName;
	string userAccountControl;
	string unicodePwd;
	string objectClass;
	string servicePrincipalName;
	class DnsHostName{
	public:
		MACHINE_ACCOUNT_ATTRIBUTE* m;
		string machineName;
		string domainName;
	};
	class SamAccountName{
	public:
		
	};
};

class LDAP_API : public MYAPI
{
private:
	string username;
	string password;
	string ldapServerAddr;
	string domainName;
	string baseDn;
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
	vector<string> LDAP_API::search(string filterGrammar, string dn, PCHAR searchAttabuite[]);
	void initBaseDn();
	void searchConstrainedDelegation();
	void searchUnconstrainedDelegation();
	void searchResourceBasedConstrainedDelegation();
	string addComputer(string pcName);
	void addDnsRecord();
	vector<string> getObjectSid(string pcName);
	void updateResourceBasedConstrainedDelegation();
	void updatePrivilege();
	void closeLdapConnection();
	SEC_WINNT_AUTH_IDENTITY_W initLdapCreds();
};

#endif