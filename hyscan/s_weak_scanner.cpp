#include "s_weak_scanner.h"
//#define DEBUG 1

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

	// domainName 大小写忽略
	string domainNameCompareString = domainName;
	// 用于比较的时候，统统都转化成小写来进行比较，因为find是区别大小写的
	transform(domainNameCompareString.begin(), domainNameCompareString.end(), domainNameCompareString.begin(), ::tolower);

	// ====== 获取主机名的IP
	HOST_ENT* remoteHost = NULL;		// 存放解析后的数据
	DWORD i, j;

	/////////////////////
	WNET_API wnetApi;
	DWORD dwSuccess = 0;

	remoteHost = gethostbyname(serverName.c_str());
	if (remoteHost != NULL){
		if (remoteHost->h_addrtype == AF_INET){
			for (i = 0; remoteHost->h_addr_list[i]; i++){
				string ip = inet_ntoa(*(IN_ADDR*)remoteHost->h_addr_list[i]);
				// ===== 判断是否存活
				dwRet = SendARP(inet_addr(ip.c_str()), 0, &MacAddr, &PhysAddrLen);
				if (dwRet == NO_ERROR){
#ifdef DEBUG
				//	cout << "[+] " << ip << " is alive" << endl;
#else
#endif
					vector<wstring> vLocalMembers = WNET_API::getLocalGroupMembers(string2Wstring(ip), L"administrators");
					for (j = 0; j<vLocalMembers.size(); j++){
						string accountString = string(wchar2Char((WCHAR*)vLocalMembers[j].c_str()));
						string accountCompareString = accountString; // 用于比较的时候，统统都转化成小写来进行比较，因为find是区别大小写的
						transform(accountCompareString.begin(), accountCompareString.end(), accountCompareString.begin(), ::tolower);
						printf("account: %s\n", accountCompareString.data());
						// 当前accountString为域组的情况下
						if (accountCompareString.find("\\") != string::npos 
							&& accountCompareString.find(domainNameCompareString) != string::npos){
							
							if (accountString.find("domain users") != string::npos){
								printf("[+] %s Domain Users\n", ip.data());
								// domain users 的情况下
								// 两种情况，一种提供了密码，一种没有提供密码
								if (!domainPassowrd.empty()){
									// 提供了账号，密码的情况下
									dwRet = wnetApi.openConnectByUserPass(ip, domainUsername, domainPassowrd);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										printf("[+] %s %s %s/%s Successed\n", ip.data(), accountString.data(), domainUsername.data(), domainPassowrd.data());
									}else{
#ifdef DEBUG
										printf("[-] %s %s %s/%s Failed\n", ip.data(), accountString.data(), domainUsername.data(), domainPassowrd.data());
#else
#endif
									}
									if (dwSuccess == 1)
										break;
								}
								else if (domainPassowrd.empty()){
									// 提供了账号，无密码的情况下，自身默认凭证去请求，这种不出意外肯定可以，因为默认凭证域中默认是连接成功的
									dwRet = wnetApi.openConnectBySelf(ip);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										printf("[+] %s %s %s/(self) Successed\n", ip.data(), accountString.data(), domainUsername.data());
									}else{
#ifdef DEBUG
										printf("[-] %s %s %s/(self) Failed\n", ip.data(), accountString.data(), domainUsername.data());
#else
#endif
									}
									if (dwSuccess == 1)
										break;
								}
							}
							else if (accountCompareString.find("domain admins") != string::npos){
								// 默认域控的本地组用户，这里跳过
								printf("[+] %s Domain Admins -> Skip\n", ip.data());
								continue;
							}
							else if (accountCompareString.find("enterprise admins") != string::npos){
								// 默认域控的本地组用户，这里跳过
								printf("[+] %s Enterprise Admins -> Skip\n", ip.data());
								continue;
							}
							else if (accountCompareString.find("administrator") != string::npos){
								// 默认域控 HENGGE\administrator，既是域管也是本地组的管理员，这种情况也需要来枚举测试
								DWORD k;
								printf("[+] Domain\\Administrator -> %s\n", accountString.data());
								vector<string> vPasswordArray = {
									R"(admin@123)", R"(pass!@#QWE)" R"(123456)", R"(P@ssw0rd)", personalPassword.c_str() };
								for (k = 0; k < vPasswordArray.size(); k++){
									dwRet = wnetApi.openConnectByUserPass(ip, "Administrator", vPasswordArray[k]);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										printf("[+] %s %s %s Successed\n", ip.data(), accountString.data(), vPasswordArray[k].data());
										break;
									}
									else{
#ifdef DEBUG
										printf("[+] %s %s %s Failed\n", ip.data(), accountString.data(), vPasswordArray[k].data());

#else
#endif
									}
								}
								if (dwSuccess == 1)
									break;
							}
							else if (accountCompareString.find(domainNameCompareString) != string::npos){
								// administrators组中有 hengge\ske 这样的用户，而不是domain users这样的组，这种域单用户的话也需要枚举测试
								// 同样分为两种情况

								// 域单用户为当前 hengge\ske 正好为当前用户
								// 域单用户为当前 hengge\ske 不是为当前用户，这种没有必要
								dwRet = wnetApi.openConnectBySelf(ip);
								if (dwRet == IPC_SUCCESS){
									dwSuccess = 1;
									// cout << "[+] " << ip << " " << domainUsername << "/(self)" << " Successed" << endl;
									printf("[+] %s %s/(self) Successed\n", ip.data(), domainUsername.data());

								}
								else{
#ifdef DEBUG
									printf("[+] %s %s/(self) Failed\n", ip.data(), domainUsername.data());

#else
#endif
								}
								if (dwSuccess == 1)
									break;
							}
							else if (accountCompareString.find("domain admins") == string::npos
								&& accountCompareString.find("enterprise admins") == string::npos){
								// 存在空格，未知domain Group的情况下
								printf("[+] %s Unknown Groups -> %s\n", ip.data(), accountString.data());
							}
						}
						else{
							// 当前accountString为本地组的情况下
							if (accountCompareString.find("administrator") != string::npos){
								// 正常本地组的administrator成员
								DWORD k;
								// cout << "[+] " << accountString << " -> Administrator...." << endl;
								printf("[+] %s -> Administrator... \n", accountString.data());
								vector<string> vPasswordArray = { R"(admin@123)", R"(pass!@#QWE)", R"(123456)",
									R"(P@ssw0rd)", personalPassword.c_str() };
								for (k = 0; k < vPasswordArray.size(); k++){
									dwRet = wnetApi.openConnectByUserPass(ip, "Administrator", vPasswordArray[k]);
									// cout << "===" << dwRet << endl;
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										printf("[+] %s %s/%s Successed\n", ip.data(), accountString.data(), vPasswordArray[k].data());
										break;
									}
									else{
#ifdef DEBUG
										printf("[+] %s %s/%s Failed\n", ip.data(), accountString.data(), vPasswordArray[k].data());
#else
#endif
									}
								}	
								if (dwSuccess == 1)
									break;
							}
							else if (accountCompareString.find(domainNameCompareString) == string::npos){
								DWORD p;
								// 这种情况就默认是administrators组下的不是administrator的用户
								// cout << "[+] " << ip << " " << accountString << " -> not Administrator...." << endl;
								printf("[+] %s %s -> not Administrator\n", ip.data(), accountString.data());
								vector<string> vString = split(accountString, R"(\)");
								if (vString.size() == 2){
									vector<string> vPasswordArray{ vString[1], vString[1] + R"(@123)",
										R"(admin@123)", R"(pass!@#QWE)", R"(123456)", R"(P@ssw0rd)", personalPassword.c_str() };
									for (p = 0; p<vPasswordArray.size(); p++){
										dwRet = wnetApi.openConnectByUserPass(ip, accountString, vPasswordArray[p]);
										if (dwRet == IPC_SUCCESS){
											dwSuccess = 1;
											printf("[+] %s %s/%s Successed\n", ip.data(), accountString.data(), vPasswordArray[p].data());
											break;
										}
										else{
#ifdef DEBUG
											printf("[+] %s %s/%s Failed\n", ip.data(), accountString.data(), vPasswordArray[p].data());
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
