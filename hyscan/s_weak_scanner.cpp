#include "s_weak_scanner.h"


WeakScanner::WeakScanner()
{

}


WeakScanner::~WeakScanner()
{

}

void WeakScanner::check(string& serverName, string& domainName, string& domainUsername, string& domainPassowrd, string& personalPassword){
	DWORD dwRet;
	ULONG dstMac[2] = { 0 };
	ULONG MacAddr[2] = { 0 };
	ULONG PhysAddrLen = 6;

	// ====== 获取本地主机名
	/*
	TCHAR hostName[MAX_PATH] = { 0 };
	gethostname(hostName, sizeof(hostName));
	cout << hostName << endl;*/

	// domainName 大小写忽略


	// ====== 获取主机名的IP
	HOST_ENT* remoteHost = NULL;		// 存放解析后的数据
	remoteHost = gethostbyname(serverName.c_str());
	if (remoteHost != NULL){
		if (remoteHost->h_addrtype == AF_INET){
			DWORD i;
			for (i = 0; remoteHost->h_addr_list[i]; i++){
				string ip = inet_ntoa(*(IN_ADDR*)remoteHost->h_addr_list[i]);
				// ===== 判断是否存活
				dwRet = SendARP(inet_addr(ip.c_str()), 0, &MacAddr, &PhysAddrLen);
				if (dwRet == NO_ERROR){
#ifdef DEBUG
					cout << "[+] " << ip << " is alive" << endl;
#else
#endif
					DWORD j;
					vector<wstring> vLocalMembers = WNET_API::getLocalGroupMembers(string2Wstring(ip), L"administrators");
					for (j = 0; j<vLocalMembers.size(); j++){

						string accountString = string(wchar2Char((WCHAR*)vLocalMembers[j].c_str()));

						// 当前accountString为域组的情况下
						if (accountString.find(R"(\)") != string::npos && accountString.find(domainName) != string::npos){
							if (accountString.find("Domain Users") != string::npos
								|| accountString.find("domain users") != string::npos){
								// cout << "[+] " << ip << " Domain Users " << endl;
								// domain users 的情况下
								// 两种情况，一种提供了密码，一种没有提供密码
								DWORD dwRet;
								DWORD dwSuccess = 0;
								WNET_API wnetApi;
								if (!domainPassowrd.empty()){
									// 提供了账号，密码的情况下
									dwRet = wnetApi.openConnectByUserPass(ip, domainUsername, domainPassowrd);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										cout << "[+] " << ip << " " << accountString << " " << domainUsername << "/" << domainPassowrd << " Successed" << endl;
									}
									if (dwSuccess == 1)
										break;
								}
								else if (domainPassowrd.empty()){
									// 提供了账号，无密码的情况下，自身默认凭证去请求，这种不出意外肯定可以，因为默认凭证域中默认是连接成功的
									dwRet = wnetApi.openConnectBySelf(ip);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										cout << "[+] " << ip << " " << accountString << " " << domainUsername << "/(self)" << " Successed" << endl;
									}
									else{
#ifdef DEBUG
										cout << "[+] " << ip << " " << accountString << " " << domainUsername << "/(self)" << " Failed" << endl;
#else
#endif
									}
									if (dwSuccess == 1)
										break;
								}
							}
							else if (accountString.find("Domain Admins") != string::npos){
								// 默认域控的本地组用户，这里跳过
								cout << "[+] " << ip << " Domain Admins -> " << "Skip" << endl;
								continue;
							}
							else if (accountString.find("Enterprise Admins") != string::npos){
								// 默认域控的本地组用户，这里跳过
								cout << "[+] " << ip << " Enterprise Admins -> " << "Skip" << endl;
								continue;
							}
							else if (accountString.find("Administrator") != string::npos){
								// 默认域控 HENGGE\administrator，既是域管也是本地组的管理员，这种情况也需要来枚举测试
								DWORD dwRet;
								DWORD dwSuccess = 0;
								DWORD k;
								cout << "[+] " << "Domain\\Administrator -> " << accountString << endl;
								WNET_API wnetApi;
								// R"(pass!@#QWE)" R"(123456)" R"(P@ssw0rd)"
								vector<string> vPasswordArray = {
									R"(admin@123)", R"(pass!@#QWE)" R"(123456)", R"(P@ssw0rd)", personalPassword.c_str() };
								for (k = 0; k < vPasswordArray.size(); k++){
									dwRet = wnetApi.openConnectByUserPass(ip, "Administrator", vPasswordArray[k]);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										cout << "[+] " << ip << " " << accountString << " " << vPasswordArray[k] << " Successed" << endl;
										break;
									}
									else{
#ifdef DEBUG
										cout << "[+] " << ip << " " << accountString << " " << vPasswordArray[k] << " Failed" << endl;

#else
#endif
									}
								}
								if (dwSuccess == 1)
									break;
							}
							else if (accountString.find(domainUsername) != string::npos){
								// administrators组中有 hengge\ske 这样的用户，而不是domain users这样的组，这种域单用户的话也需要枚举测试
								// 同样分为两种情况

								// 域单用户为当前 hengge\ske 正好为当前用户
								// 域单用户为当前 hengge\ske 不是为当前用户，这种没有必要
								DWORD dwRet;
								DWORD dwSuccess = 0;
								WNET_API wnetApi;
								dwRet = wnetApi.openConnectBySelf(ip);
								if (dwRet == IPC_SUCCESS){
									dwSuccess = 1;
									cout << "[+] " << ip << " " << domainUsername << "/(self)" << " Successed" << endl;
								}
								else{
#ifdef DEBUG
									cout << "[+] " << ip << " " << domainUsername << "/(self)" << " Failed" << endl;

#else
#endif
								}
								if (dwSuccess == 1)
									break;
							}
							else if (accountString.find(" ") != string::npos){
								// 存在空格，未知domain Group的情况下
								cout << "[+] " << ip << " Unknown Groups -> " << accountString << endl;
							}
						}
						else{
							// 当前accountString为本地组的情况下
							if (accountString.find("Administrator") != string::npos){
								// 正常本地组的administrator成员
								DWORD dwRet;
								DWORD dwSuccess = 0;
								DWORD k;
								// cout << "[+] " << accountString << " -> Administrator...." << endl;
								WNET_API wnetApi;
								vector<string> vPasswordArray = { R"(admin@123)", R"(pass!@#QWE)" R"(123456)",
									R"(P@ssw0rd)", personalPassword.c_str() };
								for (k = 0; k < vPasswordArray.size(); k++){
									dwRet = wnetApi.openConnectByUserPass(ip, "Administrator", vPasswordArray[k]);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										cout << "[+] " << ip << " " << accountString << "/" << vPasswordArray[k] << " Successed" << endl;
										break;
									}
									else{
#ifdef DEBUG
										cout << "[+] " << ip << " " << accountString << "/" << vPasswordArray[k] << " Failed" << endl;
#else
#endif
									}
								}
								if (dwSuccess == 1)
									break;
							}
							else if (accountString.find(domainName) == string::npos){
								DWORD dwRet;
								DWORD dwSuccess = 0;
								DWORD p;
								// 这种情况就默认是administrators组下的不是administrator的用户
								// cout << "[+] " << ip << " " << accountString << " -> not Administrator...." << endl;
								vector<string> vString = split(accountString, R"(\)");
								if (vString.size() == 2){
									WNET_API wnetApi;
									vector<string> vPasswordArray = { vString[1] + R"(@123)", vString[1],
										R"(admin@123)", R"(pass!@#QWE)", R"(123456)", R"(P@ssw0rd)", personalPassword.c_str() };
									for (p = 0; p<vPasswordArray.size(); p++){
										dwRet = wnetApi.openConnectByUserPass(ip, vString[1], vPasswordArray[p]);
										if (dwRet == IPC_SUCCESS){
											dwSuccess = 1;
											cout << "[+] " << ip << " " << accountString << "/" << vPasswordArray[p] << " Successed" << endl;
											break;
										}
										else{
#ifdef DEBUG
											cout << "[+] " << ip << " " << accountString << "/" << vPasswordArray[p] << " Failed" << endl;
#else
#endif
										}
									}
									if (dwSuccess == 1)
										break;
								}
							}
						}
					}
				}
			}
		}
	}
}
