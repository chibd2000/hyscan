#include "m_ldap_exp.h"
#include "m_ldap_api.h"
extern mutex g_vServiceMutex;
extern vector<ServiceVuln> g_vServiceVuln;
m_ldap_exp::m_ldap_exp(PortService portService)
{
	this->portService = portService;
}


m_ldap_exp::~m_ldap_exp()
{
	
}

void m_ldap_exp::checkServiceIsVuln(){
	if (!checkAliveReturn(this->portService.serviceIpAddr)){
		this->checkUnauth();
	}
}

void m_ldap_exp::checkUnauth(){
	/*
	get namingContexts method,for example :

	Initialize an LDAP session by calling the ldap_init() or prldap_init() function.
	Turn off automatic referral handling by calling the ldap_set_option() function and setting the LDAP_OPT_REFERRALS option to LDAP_OPT_OFF.
	Search the directory with the following criteria:
	Set the search scope to a base search.
	Specify an empty string for the base DN.
	Use the search filter (objectclass=*).
	Check the results of the search.
	If the server returns a result code, such as LDAP_OPERATIONS_ERROR, LDAP_PROTOCOL_ERROR, LDAP_REFERRAL, or LDAP_NO_SUCH_OBJECT, the LDAP server probably does not support LDAP v3.
	*/
	string bugName = "unauth";
	char szBuffer[100] = { 0 };
	LDAP* pLdapInstance;
	pLdapInstance = ldap_init((PSTR)this->portService.serviceIpAddr.c_str(), LDAP_PORT);
	if (pLdapInstance == NULL){return;}
	ldap_set_option(pLdapInstance, LDAP_OPT_PROTOCOL_VERSION, (void*)LDAP_VERSION3);
	ldap_set_option(pLdapInstance, LDAP_OPT_REFERRALS, LDAP_OPT_ON);
	if (ldap_simple_bind_s(pLdapInstance, NULL, NULL) == LDAP_SUCCESS){
		LDAPMessage* pLdapMessage;
		PCHAR pAttributes[] = { "namingContexts", NULL };
		if (ldap_search_ext_s(pLdapInstance, NULL, LDAP_SCOPE_BASE, "(objectClass=*)", pAttributes, 0,
			NULL, NULL, NULL, -1, &pLdapMessage) == LDAP_SUCCESS){
			DWORD numberOfEntries = ldap_count_entries(
				pLdapInstance,    // Session handle
				pLdapMessage);     // Search result
			if (numberOfEntries){
				memset(szBuffer, 0, 100);
				sprintf(szBuffer, "%s %s %s", this->portService.serviceIpAddr.data(), this->portService.serviceNameString.data(), bugName.data());
				g_vServiceMutex.lock();
				g_vServiceVuln.push_back(ServiceVuln(this->portService.serviceNameString, true, true, this->portService.dwServicePort, szBuffer));
				g_vServiceMutex.unlock();
			}
		}
	}
}
