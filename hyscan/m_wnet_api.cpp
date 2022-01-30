#include "m_wnet_api.h"

#define MAX_KEY_LENGTH 255

WNET_API::WNET_API()
{
	
}

WNET_API::~WNET_API()
{
	
}
 
DWORD WNET_API::openConnectBySelf(string& serverName){
	NETRESOURCE netResource;
	RtlZeroMemory(&netResource, sizeof(NETRESOURCE));
	string fmtRemoteName;
	fmtRemoteName.append("\\\\");
	fmtRemoteName.append(serverName);
	fmtRemoteName.append("\\c$");
	netResource.dwType = RESOURCETYPE_ANY;
	netResource.lpLocalName = NULL;
	netResource.lpRemoteName = (LPSTR)fmtRemoteName.c_str();
	netResource.lpProvider = NULL;
	
	DWORD dwRet = WNetAddConnection2(&netResource, NULL, NULL, CONNECT_TEMPORARY);
	if (dwRet == NO_ERROR){
		this->closeConnection(fmtRemoteName);
	}
	return dwRet;
}

DWORD WNET_API::openConnectByUserPass(string serverName, string username, string password){
	NETRESOURCE netResource;
	RtlZeroMemory(&netResource, sizeof(NETRESOURCE));
	string fmtRemoteName;
	fmtRemoteName.append("\\\\");
	fmtRemoteName.append(serverName);
	fmtRemoteName.append("\\c$");
	netResource.dwType = RESOURCETYPE_ANY;
	netResource.lpLocalName = NULL;
	netResource.lpRemoteName = (LPSTR)fmtRemoteName.c_str();
	netResource.lpProvider = NULL;
	DWORD dwRet = WNetAddConnection2(&netResource, password.c_str(), username.c_str(), CONNECT_TEMPORARY);
	// cout << dwRet << endl;
	switch (dwRet)
	{
	case IPC_SUCCESS:
		// printf("[+] %s Connection success\n", netResource.lpRemoteName);
		this->closeConnection(fmtRemoteName);
		break;
	case IPC_PRIVILEGE_ERROR:
		printf("[-] %s The Privilege wrong, but it can Successed\n", netResource.lpRemoteName);
		break;

	case IPC_NETWORK_ERROR:
		printf("[-] %s The Network name could not be found\n", netResource.lpRemoteName);
		break;
	case IPC_Credential_Conflict:
		printf("[-] %s The Credential is conflict, but it can successed\n", netResource.lpRemoteName);
		break;
	case IPC_USER_PASS_ERROR:
		printf("[-] %s The username or password is incorrect\n", netResource.lpRemoteName);
		break;
	case IPC_PASS_EXPIRE:
		printf("[-] %s The password is expired, please use smbpasswd\n", netResource.lpRemoteName);
		break;
	default:
		//printf("[-] %s WNetAddConnection2 failed with error %d\n", netResource.lpRemoteName, GetLastError());
		break;
	}
	
	return dwRet;
}

DWORD WNET_API::closeConnection(string lpRemoteName){
	DWORD dwRetVal;
	dwRetVal = WNetCancelConnection2(lpRemoteName.c_str(), 0, TRUE);

	if (dwRetVal == NO_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void WNET_API::getNetSession(wstring& serverName){
	NET_API_STATUS nStatus;
	LPSESSION_INFO_10 pBuf = NULL;
	LPSESSION_INFO_10 pTmpBuf;
	DWORD dwLevel = 10;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;

	nStatus = NetSessionEnum((LPWSTR)serverName.c_str(), NULL, NULL, dwLevel, 
		(LPBYTE*)&pBuf, 
		dwPrefMaxLen,
		&dwEntriesRead,
		&dwTotalEntries,
		&dwResumeHandle);

	if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
	{
		if ((pTmpBuf = pBuf) != NULL)
		{
			wprintf(L"[+] \\\\%s\n", serverName.data());
			for (i = 0; i < dwEntriesRead; i++)
			{

				if (pTmpBuf == NULL)
				{
					printf("[-] WNET_API::NetSessionEnum Error, The Error is %d\n", nStatus);
					break;
				}
				
				wprintf(L"\tClient:%s User:%s Active:%d \n", pTmpBuf->sesi10_cname, pTmpBuf->sesi10_username, pTmpBuf->sesi10_time);
				pTmpBuf++;
			}
			if (pBuf != NULL){
				NetApiBufferFree(pBuf);
			}
		}
	}
}

void WNET_API::getNetShare(wstring& serverName){
	DWORD dwMaxLen = MAX_PREFERRED_LENGTH;
	PSHARE_INFO_502 si502,pTmp;
	DWORD dwEntriesRead;
	DWORD dwTotalEntries;
	DWORD dwResumeHandle;
	NET_API_STATUS nStatus;
	nStatus = NetShareEnum((LPWSTR)serverName.c_str(), 502, (LPBYTE*)&si502, dwMaxLen, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);
	if (nStatus == ERROR_SUCCESS || nStatus == ERROR_MORE_DATA)
	{
		if (dwEntriesRead > 0){
			wprintf(L"[+] \\\\%s\n", serverName.c_str());
			pTmp = si502;
			for (DWORD i = 0; i < dwEntriesRead; i++)
			{
				printf("\tshi502_netname:%s shi502_path:%s shi502_current_uses:%s\n", pTmp->shi502_netname, pTmp->shi502_path, pTmp->shi502_current_uses);
				pTmp++;
			}
			NetApiBufferFree(si502);
		}
	}
}

// enum HKU registry values来获取相关信息，用于低权限定位
// 缺点就是PC机器用不了这种方法
void WNET_API::getLoggedUsers(string& serverName){
	// HKEY_USERS
	HKEY hKey = NULL;
	DWORD dwRet;
	dwRet = RegConnectRegistry(serverName.c_str(), HKEY_USERS, &hKey);
	if (dwRet != ERROR_SUCCESS)
	{
		printf("[-] WNET_API::RegConnectRegistry Failed, The Error is %d\n", dwRet);
		return;
	}
	DWORD dwSubKeyNum;

	dwRet = RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwSubKeyNum, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	if (dwRet != ERROR_SUCCESS){
		printf("[-] WNET_API::RegQueryInfoKey Failed, The Error is %d\n", dwRet);
		return;
	}
	
	FILETIME ft;
	TCHAR tChName[MAX_KEY_LENGTH] = { 0 };
	DWORD dwChName;
	vector<string> vLogged;
	for (DWORD i = 0;i<dwSubKeyNum;i++){
		dwChName = MAX_KEY_LENGTH;
		dwRet = RegEnumKeyEx(hKey, i, tChName, &dwChName, NULL, NULL, NULL, &ft);
		if (dwRet != ERROR_SUCCESS){
			printf("[-] WNET_API::RegEnumKeyEx Failed, The Error is %d\n", dwRet);
		}
		if (regex_match(tChName, regex("S-1-5-21-[0-9]+-[0-9]+-[0-9]+-[0-9]+$"), regex_constants::format_first_only)){
			vLogged.push_back(tChName);
		}
		RtlZeroMemory(tChName, 0);
	}

	RegCloseKey(hKey);
	/// ---------------
	//string logString;
	printf("[+] \\\\%s\n", serverName.data());
	//logString = "[+] \\\\" + serverName;
	//printf("%s",logString.data());
	//m_logger::pLogger->info(logString);

	PSID pSid = NULL;
	for each (string sidString in vLogged)
	{
		ConvertStringSidToSid(sidString.c_str(), &pSid);
		string usernameString = sid2user(pSid, NULL);
		printf("\t %s ---- %s\n", sidString.data(), usernameString.data());
		//printf("%s")
		//logString = "\t " + sidString + " ---- " + usernameString;
		//printf("%s\n", logString.data());
		//m_logger::pLogger->info(logString);
	}
}

// @samlib @SAMR
// @param serverName
// @param groupName

//vector<wstring> WNET_API::getLocalGroupMembersSamr(wstring serverName){
	/*
	vector<wstring> membersList;
	UNICODE_STRING* pServerNameUnicodeString = NULL;
	RtlInitUnicodeString(pServerNameUnicodeString, serverName.c_str());
	SAMPR_HANDLE serverHandle;
	NTSTATUS ntStatus;
	ntStatus = SamConnect(pServerNameUnicodeString, &serverHandle, SAM_SERVER_CONNECT | SAM_SERVER_LOOKUP_DOMAIN, true);
	if (!BCRYPT_SUCCESS(ntStatus)){
		return membersList;
	}

	SAMPR_HANDLE domainHandle;
	PPOLICY_ACCOUNT_DOMAIN_INFO domainInfo = NULL;
	LSA_OBJECT_ATTRIBUTES ObjectAttributes = { 0 };
	LSA_HANDLE hPolicy = NULL;

	LsaOpenPolicy(NULL, &ObjectAttributes, POLICY_VIEW_LOCAL_INFORMATION, &hPolicy);
	LsaQueryInformationPolicy(hPolicy, PolicyAccountDomainInformation, (PVOID*)&domainInfo);
	ntStatus = SamOpenDomain(serverHandle, DOMAIN_ALL_ACCESS, domainInfo->DomainSid, &domainHandle);
	if (!BCRYPT_SUCCESS(ntStatus)){
		SamCloseHandle(serverHandle);
		return membersList;
	}
	// SamOpenAlias(IN SAMPR_HANDLE DomainHandle, IN ACCESS_MASK DesiredAccess, IN DWORD AliasId, OUT SAMPR_HANDLE* AliasHandle);
	SAMPR_HANDLE aliasHandle;
	ntStatus = SamOpenAlias(domainHandle, STANDARD_RIGHTS_ALL, 544, &aliasHandle);
	if (!BCRYPT_SUCCESS(ntStatus)){
		SamCloseHandle(serverHandle);
		SamCloseHandle(domainHandle);
		return membersList;
	}
	SAMPR_HANDLE groupHandle;
	ntStatus = SamOpenGroup(domainHandle, GROUP_ALL_ACCESS, 544, &groupHandle);
	PSAMPR_GET_MEMBERS_BUFFER membersBuffer;
	SamGetMembersInGroup(groupHandle, &membersBuffer);

	////////

	return membersList;*/
//}

/////////////////////// @ske大师兄
// @param serverName
// @param groupName
vector<wstring> WNET_API::getLocalGroupMembers(wstring serverName, wstring groupName){
	LOCALGROUP_MEMBERS_INFO_2* pGroupInfo;			// LOCALGROUP_MEMBERS_INFO_2结构获得返回与本地组成员关联的SID、帐户信息和域名，变量buff存放获取到的信息
	DWORD dwPrefmaxlen = MAX_PREFERRED_LENGTH;	// 指定返回数据的首选最大长度，以字节为单位。如果指定MAX_PREFERRED_LENGTH，该函数将分配数据所需的内存量。
	DWORD dwEntriesRead;						// 指向一个值的指针，该值接收实际枚举的元素数。
	DWORD dwTotalentries;						//指向一个值的值，该值接收可能已从当前简历位置枚举的条目总数
	vector<wstring> membersList;
	DWORD dwRet;
	dwRet = NetLocalGroupGetMembers(serverName.c_str(), 
		groupName.c_str(), 2, (LPBYTE*)&pGroupInfo, dwPrefmaxlen, &dwEntriesRead, &dwTotalentries, NULL);
	if (dwRet == NO_ERROR)
	{
		for (DWORD i = 0; i < dwEntriesRead; i++){
			// wcout << pGroupInfo[i].lgrmi2_domainandname << " ";
			membersList.push_back(pGroupInfo[i].lgrmi2_domainandname);
		}
		// cout << endl;
	}else{
		printf("[-] %s WNET_API::NetLocalGroupGetMembers Error, The Error is %d\n", serverName.data(), dwRet);
	}
	return membersList;
}


/////////////////////// @ske大师兄
// @param serverName
// @param groupName
vector<wstring> WNET_API::getDomainGroupMembers(wstring serverName, wstring groupName){
	DWORD dwLevel = 1;
	GROUP_USERS_INFO_1* bufptr;
	DWORD dwPrefmaxlen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesread;
	DWORD dwTotalentries;
	DWORD dwRet;
	vector<wstring> membersList;				// 定义vector，存放主机名

	dwRet = NetGroupGetUsers(serverName.c_str(), groupName.c_str(), dwLevel, (LPBYTE*)&bufptr, dwPrefmaxlen, &dwEntriesread, &dwTotalentries, NULL);
	
	if (dwRet == NO_ERROR)
	{
		for (DWORD i = 0; i < dwEntriesread; i++)
		{
			wstring nameString(bufptr[i].grui1_name);
			if (*nameString.rbegin() == '$')
				nameString.replace(nameString.end() - 1, nameString.end(), 1, NULL);			// 主机名最末尾的$替换为空
			// wcout << nameString << endl;
			membersList.push_back(nameString.c_str());
		}
		return membersList;
	}
	else
	{
		printf("[-] %s WNET_API::NetGroupGetUsers Error, The Error is %d\n", serverName.data(), dwRet);
		exit(-1);
	}
}



