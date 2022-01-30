#include "s_weak_scanner.h"
#define DEBUG 1

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
#endif
					vector<wstring> vLocalMembers = WNET_API::getLocalGroupMembers(string2Wstring(ip), L"administrators");
					for (j = 0; j<vLocalMembers.size(); j++){
						string accountString = string(wchar2Char((WCHAR*)vLocalMembers[j].c_str()));
						string accountCompareString = accountString; // 用于比较的时候，统统都转化成小写来进行比较，因为find是区别大小写的
						transform(accountCompareString.begin(), accountCompareString.end(), accountCompareString.begin(), ::tolower);
						printf("[+] %s Current Account: %s\n", ip.data(), accountString.data());
						// 当前accountString为域组的情况下
						if (accountCompareString.find("\\") != string::npos 
							&& accountCompareString.find(domainNameCompareString) != string::npos){
#ifdef DEBUGA
							printf("----1\n");
#endif
							if (accountCompareString.find("domain users") != string::npos){
#ifdef DEBUGA
								printf("----1-1\n");
#endif
								printf("\t[+] %s Domain Users\n", ip.data());
								// domain users 的情况下
								// 两种情况，一种提供了密码，一种没有提供密码
								if (!domainPassowrd.empty()){
									// 提供了账号，密码的情况下
									dwRet = wnetApi.openConnectByUserPass(ip, domainUsername, domainPassowrd);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										printf("\t[+] %s %s %s/%s Successed\n", ip.data(), accountString.data(), domainUsername.data(), domainPassowrd.data());
									}else{
#ifdef DEBUG
										printf("\t[-] %s %s %s/%s Failed\n", ip.data(), accountString.data(), domainUsername.data(), domainPassowrd.data());
#else
#endif
									}
									if (dwSuccess == 1)
										break;
								}
								else if (domainPassowrd.empty()){
#ifdef DEBUGA
									printf("----1-2\n");
#endif
									// 提供了账号，无密码的情况下，自身默认凭证去请求，这种不出意外肯定可以，因为默认凭证域中默认是连接成功的
									dwRet = wnetApi.openConnectBySelf(ip);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										printf("\t[+] %s %s %s/(self) Successed\n", ip.data(), accountString.data(), domainUsername.data());
									}else{
#ifdef DEBUG
										printf("\t[-] %s %s %s/(self) Failed\n", ip.data(), accountString.data(), domainUsername.data());
#endif
									}
									if (dwSuccess == 1)
										break;
								}
							}
							else if (accountCompareString.find("domain admins") != string::npos){
#ifdef DEBUGA
								printf("----1-3\n");
#endif
								// 默认域控的本地组用户，这里跳过
								printf("\t[+] %s Domain Admins -> Skip\n", ip.data());
								continue;
							}
							else if (accountCompareString.find("enterprise admins") != string::npos){
#ifdef DEBUGA
								printf("----1-4\n");
#endif
								// 默认域控的本地组用户，这里跳过
								printf("\t[+] %s Enterprise Admins -> Skip\n", ip.data());
								continue;
							}
							else if (accountCompareString.find("administrator") != string::npos){
#ifdef DEBUGA
								printf("----1-5\n");
#endif
								// 默认域控 HENGGE\administrator，既是域管也是本地组的管理员，这种情况也需要来枚举测试
								DWORD k;
								printf("\t[+] Domain\\Administrator -> %s\n", accountString.data());
								vector<string> vPasswordArray = {
									R"(admin@123)", R"(pass!@#QWE)" R"(123456)", R"(P@ssw0rd)", personalPassword.c_str() };
								for (k = 0; k < vPasswordArray.size(); k++){
									dwRet = wnetApi.openConnectByUserPass(ip, "Administrator", vPasswordArray[k]);
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										printf("\t[+] %s %s %s Successed\n", ip.data(), accountString.data(), vPasswordArray[k].data());
										break;
									}
									else{
#ifdef DEBUG
										printf("\t[+] %s %s %s Failed\n", ip.data(), accountString.data(), vPasswordArray[k].data());
#endif
									}
								}
								if (dwSuccess == 1)
									break;
							}

							else if (accountCompareString.find(domainNameCompareString) != string::npos){
								// 1、"hengge\www".find("hengge") 这种情况
								// 2、"hengge\abc".find("hengge") 这种情况
								// 3、"hengge\qqq".find("hengge") 这种情况
#ifdef DEBUGA
								printf("----1-6\n");
#endif
								// administrators组中有 hengge\ske 这样的用户，而不是domain users这样的组，这种域单用户的话也需要枚举测试
								// 同样分为两种情况

								string domainuserNameCompareString = domainUsername;
								transform(domainuserNameCompareString.begin(), domainuserNameCompareString.end(), 
									domainuserNameCompareString.begin(), ::tolower);
								if (accountCompareString.find(domainuserNameCompareString) != string::npos){
									// 1、"hengge\ske".find("hengge\ske") 这种情况
#ifdef DEBUGA
									printf("----1-7\n");
#endif
									if (domainPassowrd.empty()){
										// 如果提供的域用户名和遍历的用户相同，但是密码没有提供，那只能用当前默认的域用户凭证认证
										dwRet = wnetApi.openConnectBySelf(ip);
										if (dwRet == IPC_SUCCESS){
											dwSuccess = 1;
											printf("\t[+] %s %s/(self) Successed\n", ip.data(), domainUsername.data());
											break;
										}
									}
									else if (!domainPassowrd.empty()){
#ifdef DEBUGA
										printf("----1-8\n");
#endif
										// 如果提供的域用户名和遍历的用户相同，并且密码提供，那就用当前密码提供的来进行一次认证
										dwRet = wnetApi.openConnectByUserPass(ip, domainUsername, domainPassowrd);
										if (dwRet == IPC_SUCCESS){
											dwSuccess = 1;
											printf("\t[+] %s %s %s/%s Successed\n", ip.data(), accountString.data(), domainUsername.data(), domainPassowrd.data());
										}
										else{
#ifdef DEBUG
											printf("\t[-] %s %s %s/%s Failed\n", ip.data(), accountString.data(), domainUsername.data(), domainPassowrd.data());
#endif
										}
										if (dwSuccess == 1)
											break;
									}
								}
								else if (accountCompareString.find(domainuserNameCompareString) == string::npos){
									// 1、"hengge\cbd".find("hengge\ske") 这种情况
#ifdef DEBUGA
									printf("----1-9\n");
#endif
									// 域单用户为当前 hengge\ske 不是为当前用户
									// 这种就需要进行枚举，但是不需要用自身凭据了，因为自身凭据都对不上号
									if (!domainPassowrd.empty()){
										// 如果提供的当前域用户名和遍历出来的当前用户不同
										// 并且密码提供，那就默认的密码字典和当前提供的密码来进行枚举测试
										vector<string> vPasswordArray = { personalPassword.c_str(), domainPassowrd.c_str(),
											R"(admin@123)", R"(pass!@#QWE)", R"(123456)", R"(P@ssw0rd)"};
										for (DWORD k = 0; k < vPasswordArray.size(); k++){
											dwRet = wnetApi.openConnectByUserPass(ip, accountString, vPasswordArray[k]);
											if (dwRet == IPC_SUCCESS){
												dwSuccess = 1;
												printf("\t[+] %s %s/%s Successed\n", ip.data(), accountString.data(), vPasswordArray[k].data());
												break;
											}
											else{
#ifdef DEBUG
												printf("\t[+] %s %s/%s Failed\n", ip.data(), accountString.data(), vPasswordArray[k].data());
#endif
											}
										}
										if (dwSuccess == 1)
											break;
									}
								}
							}
							else if (accountCompareString.find("domain admins") == string::npos
								&& accountCompareString.find("enterprise admins") == string::npos){
#ifdef DEBUGA
								printf("----1-10\n");
#endif
								// 存在空格，未知domain Group的情况下
								printf("\t[+] %s Unknown Groups -> %s\n", ip.data(), accountString.data());
							}
						}
						else{
							// 当前accountString为本地组的情况下
							if (accountCompareString.find("administrator") != string::npos){
#ifdef DEBUGA
								printf("----1-11\n");
#endif
								// 正常本地组的administrator成员
								DWORD k;
								// cout << "[+] " << accountString << " -> Administrator...." << endl;
								printf("\t[+] %s -> Administrator... \n", accountString.data());
								vector<string> vPasswordArray = { R"(admin@123)", R"(pass!@#QWE)", R"(123456)",
									R"(P@ssw0rd)", personalPassword.c_str() };
								for (k = 0; k < vPasswordArray.size(); k++){
									dwRet = wnetApi.openConnectByUserPass(ip, "Administrator", vPasswordArray[k]);
									// cout << "===" << dwRet << endl;
									if (dwRet == IPC_SUCCESS){
										dwSuccess = 1;
										printf("\t[+] %s %s/%s Successed\n", ip.data(), accountString.data(), vPasswordArray[k].data());
										break;
									}
									else{
#ifdef DEBUG
										printf("\t[+] %s %s/%s Failed\n", ip.data(), accountString.data(), vPasswordArray[k].data());
#endif
									}
								}	
								if (dwSuccess == 1)
									break;
							}
							else if (accountCompareString.find(domainNameCompareString) == string::npos){
#ifdef DEBUGA
								printf("----1-12\n");
#endif
								DWORD p;
								// 这种情况就默认是administrators组下的不是administrator的用户
								// cout << "[+] " << ip << " " << accountString << " -> not Administrator...." << endl;
								printf("\t[+] %s %s -> not Administrator\n", ip.data(), accountString.data());
								vector<string> vString = split(accountString, R"(\)");
								if (vString.size() == 2){
									vector<string> vPasswordArray{ vString[1], vString[1] + R"(@123)",
										R"(admin@123)", R"(pass!@#QWE)", R"(123456)", R"(P@ssw0rd)", personalPassword.c_str() };
									for (p = 0; p<vPasswordArray.size(); p++){
										dwRet = wnetApi.openConnectByUserPass(ip, accountString, vPasswordArray[p]);
										if (dwRet == IPC_SUCCESS){
											dwSuccess = 1;
											printf("\t[+] %s %s/%s Successed\n", ip.data(), accountString.data(), vPasswordArray[p].data());
											break;
										}
										else{
#ifdef DEBUG
											printf("\t[+] %s %s/%s Failed\n", ip.data(), accountString.data(), vPasswordArray[p].data());
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
