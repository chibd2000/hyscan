#include "m_ldap_api.h"

LDAP_API::LDAP_API(string username, string password, string ldapServerAddr, string domainName)
{
	this->pLdapInstance = NULL;
	this->pLdapMessage = NULL;
	this->pEntry = NULL;
	this->ldapServerAddr = ldapServerAddr;
	this->domainName = domainName;
	this->username = username;
	this->password = password;
	this->defaultTimeVal.tv_sec = 1;
	this->init();
}
 
LDAP_API::LDAP_API(string ldapServerAddr, string domainName)
{
	this->pLdapInstance = NULL;
	this->pLdapMessage = NULL;
	this->pEntry = NULL;
	this->ldapServerAddr = ldapServerAddr;
	this->domainName = domainName;
	this->defaultTimeVal.tv_sec = 1;
	this->init();
}


LDAP_API::~LDAP_API()
{
	this->closeLdapConnection();
}

void LDAP_API::closeLdapConnection(){
	if (this->pLdapMessage != NULL)
		ldap_msgfree(this->pLdapMessage);
	if (this->pLdapInstance != NULL)
		ldap_unbind_s(this->pLdapInstance);
}


void LDAP_API::init(){
	this->pLdapInstance = ldap_init((PSTR)this->ldapServerAddr.c_str(), 389);
	if (this->setLdapOption() == LDAP_SUCCESS){
		if (this->connect() == LDAP_SUCCESS){
			if (this->bind() == LDAP_SUCCESS){
				cout << "[+] LDAP::init success" << endl;
			}
		}
	}
}

SEC_WINNT_AUTH_IDENTITY_W LDAP_API::initLdapCreds(){
	SEC_WINNT_AUTH_IDENTITY_W secAuthIdentity;
	
	secAuthIdentity.Domain = (USHORT*)this->ldapServerAddr.c_str();
	secAuthIdentity.DomainLength = lstrlen(this->ldapServerAddr.c_str());
	
	if (!this->username.empty()){
		secAuthIdentity.User = (USHORT*)this->username.c_str();
		secAuthIdentity.UserLength = lstrlen(this->username.c_str());
	}

	if (!this->password.empty()){
		secAuthIdentity.Password = (USHORT*)this->password.c_str();
		secAuthIdentity.PasswordLength = lstrlen(this->password.c_str());
	}

	secAuthIdentity.Flags = SEC_WINNT_AUTH_IDENTITY_ANSI;
	return secAuthIdentity;
}

DWORD LDAP_API::setLdapOption(){
	DWORD dwRet = ldap_set_option(this->pLdapInstance, LDAP_OPT_PROTOCOL_VERSION, (void*)LDAP_VERSION3);
	if (dwRet == LDAP_SUCCESS){
		return LDAP_SUCCESS;
#ifdef DEBUG
		cout << "[+] LDAPAPI_::setLdapOption succuss" << endl;
#else	
	}
	else{
		cout << "[-] LDAP::setLdapOption failed" << endl;
		this->closeLdapConnection();
		return LDAP_OPERATIONS_ERROR;
	}
#endif
}

DWORD LDAP_API::connect(){
	DWORD dwRet = ldap_connect(this->pLdapInstance, &defaultTimeVal);
	if (dwRet == LDAP_SUCCESS){
		return LDAP_SUCCESS;
#ifdef DEBUG
		cout << "[+] LDAPAPI_::setLdapOption succuss" << endl;
#else
	}
	else{
		cout << "[-] LDAP::connect failed" << endl;
		this->closeLdapConnection();
		return LDAP_OPERATIONS_ERROR;
	}
#endif
}

DWORD LDAP_API::bind(){
	vector<string> vString = split(this->domainName, ".");
	string szDn = "DC=" + vString[0] + ",DC=" + vString[1];
	SEC_WINNT_AUTH_IDENTITY_W secAuthIdentity = this->initLdapCreds();
	DWORD dwRet = ldap_bind_s(this->pLdapInstance, (PSTR)szDn.c_str(), (PCHAR)&secAuthIdentity, LDAP_AUTH_NEGOTIATE);
	if (dwRet != -1){
		return LDAP_SUCCESS;
#ifdef DEBUG
		cout << "[+] LDAPAPI_::bind succuss" << endl;
#else	
	}
	else{
		cout << "[-] LDAP::bind failed" << endl;
		this->closeLdapConnection();
		return LDAP_OPERATIONS_ERROR;
	}
#endif
}

/*大部分都参考了ske大师兄的，因为写的比较完全了，比如基于资源的约束委派方面上相关的SID获取用户名其实都写了*/
void LDAP_API::search(string filterGrammar, PCHAR searchAttabuite[]){
	vector<string> vString = split(this->domainName, ".");
	string szDn = "DC=" + vString[0] + ",DC=" + vString[1];
	DWORD dwRet = ldap_search_s(this->pLdapInstance, (PSTR)szDn.c_str(), LDAP_SCOPE_SUBTREE, (PSTR)filterGrammar.c_str(), 
		(PZPSTR)searchAttabuite, 0, &this->pLdapMessage);
	if (dwRet == LDAP_SUCCESS){
		DWORD numberOfEntries = ldap_count_entries(
			this->pLdapInstance,    // Session handle
			this->pLdapMessage);     // Search result

		if (numberOfEntries != NULL)
		{
			//----------------------------------------------------------
			// Loop through the search entries, get, and output the
			// requested list of attributes and values.
			//----------------------------------------------------------
			LDAPMessage* pEntry = NULL;
			PCHAR pEntryDN = NULL;
			ULONG iCnt = 0;
			BerElement* pBer = NULL;
			PCHAR pAttribute = NULL;
			PCHAR* ppValue = NULL;
			ULONG iValue = 0;
			
			for (iCnt = 0; iCnt < numberOfEntries; iCnt++)
			{
				// Get the first/next entry.
				if (!iCnt)
					pEntry = ldap_first_entry(this->pLdapInstance, this->pLdapMessage);
				else
					pEntry = ldap_next_entry(this->pLdapInstance, pEntry);
				
				if (pEntry != NULL){
					// Get the first attribute name.
					pAttribute = ldap_first_attribute(this->pLdapInstance, pEntry, &pBer);
					while (pAttribute != NULL){
						// 这种情况就是如果ppValue是sid的时候，需要手动进行转换成用户下，否则输出是未知数据
						if (CompareString(GetThreadLocale(), NORM_IGNORECASE, pAttribute, lstrlen(pAttribute), "mS-DS-CreatorSID", lstrlen("mS-DS-CreatorSID")) == 2){
							LDAP_BERVAL** berVal;
							string swSid;
							string domainUser;
							PSID pSid;
							if (berVal = ldap_get_values_len(this->pLdapInstance, pEntry, pAttribute)){
								for (int i = 0; berVal[i]; i++)
								{
									string sid = ConvertToStringSid((const unsigned char*)berVal[i]->bv_val, berVal[i]->bv_len);
									ConvertStringSidToSid(sid.c_str(), &pSid);
									domainUser = sid2user(pSid, this->ldapServerAddr.c_str());
									cout << "\t" << pAttribute << "=" << domainUser << " ";
								}
								ldap_value_free_len(berVal);
							}
						}
						else{
							// get common attribute
							ppValue = ldap_get_values(this->pLdapInstance, pEntry, pAttribute);
							cout << "\t" << pAttribute << "=" << *ppValue << " ";
						}
						
						// Free memory.
						if (ppValue != NULL)
							ldap_value_free(ppValue);
						
						ppValue = NULL;
						ldap_memfree(pAttribute);

						// Get next attribute name.
						pAttribute = ldap_next_attribute(
							this->pLdapInstance,   // Session Handle
							pEntry,            // Current entry
							pBer);             // Current BerElement
					}

					cout << endl;
				}
				else{
					this->closeLdapConnection();
				}
			}
		}else{
			cout << "[-] LDAP::ldap_count_entries failed" << endl;
			this->closeLdapConnection();
		}
		
#ifdef DEBUG
		cout << "[+] LDAPAPI_::setLdapOption succuss" << endl;
#else
	}
	else{
		cout << "[-] LDAP::search failed, error code: " << dwRet << endl;
		this->closeLdapConnection();
		// exit(-1);
	}
#endif
	
}

void LDAP_API::searchConstrainedDelegation(){
	PCHAR pAttributes[] = {"msds-allowedtodelegateto", "cn", NULL };
	this->search("(&(samAccountType=805306368)(msds-allowedtodelegateto=*))", pAttributes);
}

void LDAP_API::searchUnconstrainedDelegation(){
	PCHAR pAttributes[] = {"userAccountControl", "cn", NULL };
	this->search("(&(objectClass=computer)(userAccountControl:1.2.840.113556.1.4.803:=524288))", pAttributes);
}

void LDAP_API::searchResourceBasedConstrainedDelegation(){
	PCHAR pAttributes[] = {"mS-DS-CreatorSID", "cn", NULL };
	this->search("(&(ObjectClass=computer)(mS-DS-CreatorSID=*))", pAttributes);
}
