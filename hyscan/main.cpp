#include "public.h"
#include "m_multi_framework.h"
// test

static unsigned char b[5];
vector<NtlmInfo> vNtlmInfo; // save ntlm type2
mutex vectorNtlmInfoMutex;
vector<string> vServerLocalMemberAccount;
map<string, vector<string>> mServerLoggedAccount;

/*
后面继续改，先把要的功能稳定实现
const KUHL_M* mimikatz_modules[] = {
	&kuhl_m_standard,
	&kuhl_m_tgt,
	&kuhl_m_tgs,
	&kuhl_m_exploit,
	&kuhl_m_misc,
	&kuhl_m_kerberos,
	&kuhl_m_smb,
	&kuhl_m_ntlm,
	&kuhl_m_tsssp,
	&kuhl_m_server,
};*/

void socketInit(){
	WSADATA wsaData;
	if (!WSAStartup(MAKEWORD(2, 2), &wsaData)){
		cout << "[+] socketInit Successed" << endl;
	}
	else{
		cout << "[-] socketInit Failed" << endl;
	}
}

void getBanner(){
	cout << "[+] Welcome from HengGe Team :)" << endl;
}

void getConsole(int argc, char* argv[]){
	// console https://github.com/tanakh/cmdline/issues/6
	cout << "[+] getConsole..." << endl;
}

void testFunc(void* arg)
{
	ThreadPool* threadPool = (ThreadPool*)arg;
	cout << "[+] TaskSize		----> " << threadPool->getQueueTaskSize() << endl;
	cout << "[+] workThreadNum	---->" << threadPool->workThreadNum << endl;
	cout << "[+] aliveThreadNum ----> " << threadPool->aliveThreadNum << endl;
	Sleep(1000);
}

void getIpList(PDWORD startIp, PDWORD lastIp){
	DWORD ip = (b[0] << 24UL) | (b[1] << 16UL) | (b[2] << 8UL) | (b[3]);
	DWORD mask = (0xFFFFFFFFUL << (32 - b[4])) & 0xFFFFFFFFUL;
	*startIp = ip & mask;;
	*lastIp = *startIp | ~mask; //   111 1111 1111 0000  |  0000 0000 0000 1111
}


int main(int argc, char* argv[]){	
	/* write in 2021.12.09 15.43 @zpchcbd for WMINtlmScan study*/
	getBanner();
	
	/////////////////TEST CODE
	socketInit();
	DWORD startIp = 0;
	DWORD lastIp = 0;
	ScannerFactory scannerFactory;
	m_multi_framework threadFrame(100);

	
	if (argc == 3){
		if (strcmp("-cidr", argv[1]) == 0){
			// hyscan -cidr 192.168.0.108/24
			sscanf(argv[2], "%hhu.%hhu.%hhu.%hhu/%hhu", &b[0], &b[1], &b[2], &b[3], &b[4]);
			if (b[4] > 32)
				return -1;
			getIpList(&startIp, &lastIp);
			for (UINT32 i = startIp; i <= lastIp; i++){
				map<string, string>* mArgs = new map<string, string>;
				mArgs->insert(pair<string, string>("ipAddr", int2ip(i)));
				threadFrame.addTask(WMIScanner::check, mArgs);
			}
		}else if (strcmp("-c", argv[1]) == 0){
			// hyscan -c 192.168.0.108
			map<string, string>* mArgs = new map<string, string>;
			mArgs->insert(pair<string, string>("ipAddr", argv[2]));
			threadFrame.addTask(WMIScanner::check, mArgs);
		}else if (strcmp("-weak", argv[1]) == 0){
			string domainName = "HENGGE";
			string domainUsername = "hengge\\ske";
			string domainPassword = "";
			string personalPassword = "admin@332211";
			vector<wstring> vComputerMembers = WNET_API::getDomainGroupMembers(L"192.168.4.11", L"Domain Computers");
			vector<wstring> vControllerMembers = WNET_API::getDomainGroupMembers(L"192.168.4.11", L"Domain Controllers");
			vector<wstring> vMembers;
			vMembers.insert(vMembers.end(), vComputerMembers.begin(), vComputerMembers.end());
			vMembers.insert(vMembers.end(), vControllerMembers.begin(), vControllerMembers.end());
			DWORD i;
			for (i = 0; i<vMembers.size(); i++){
				map<string, string>* mArgs = new map<string, string>;
				mArgs->insert(pair<string, string>("serverName", wchar2Char((WCHAR*)vMembers[i].c_str())));
				mArgs->insert(pair<string, string>("domainName", domainName));
				mArgs->insert(pair<string, string>("domainUsername", domainUsername));
				mArgs->insert(pair<string, string>("domainPassword", domainPassword));
				mArgs->insert(pair<string, string>("personalPassword", personalPassword));
				threadFrame.addTask(WeakScanner::check, mArgs);
			}

		}else{
			
		}
	}
	else if (argc == 2){
		 
	}

	threadFrame.startWork();
	threadFrame.getNtlmInfoResult();
	
	
	
/*
	ScannerFactory* scannerFactory = new ScannerFactory();
	BaseScanner* scanner = nullptr;
	
	if (argc == 4){
		if (strcmp("wmi::scan", argv[1]) == 0 && strcmp("-c", argv[2]) == 0){
			scanner = scannerFactory->createScanner("WMIScanner");
			threadList.push_back(thread(threadCheckFunc, scanner, "192.168.0.108"));
			threadList[0].join();
		}else if (strcmp("wmi::pth", argv[1]) == 0 && strcmp("-c", argv[2]) == 0){
			scanner = scannerFactory->createScanner("WMIScanner");
		}else if (strcmp("smb::scan", argv[1]) == 0 && strcmp("-c", argv[2]) == 0){
			scanner = scannerFactory->createScanner("SMBScanner");
		}else if (strcmp("smb::pth", argv[1]) == 0 && strcmp("-c", argv[2]) == 0){
			scanner = scannerFactory->createScanner("SMBScanner");
		}else if (strcmp("winrm::scan", argv[1]) == 0 && strcmp("-c", argv[2]) == 0){
			scanner = scannerFactory->createScanner("WinrmScanner");
		}else if (strcmp("oxid::scan", argv[1]) == 0 && strcmp("-c", argv[2]) == 0){
			scanner = scannerFactory->createScanner("OXIDScanner");
		}else if (strcmp("ldap::delegation", argv[1]) == 0 && strcmp("-c", argv[2]) == 0){
			scanner = scannerFactory->createScanner("OXIDScanner");
		}else if (strcmp("kerberos::enum::user", argv[1]) == 0){
			scanner = scannerFactory->createScanner("OXIDScanner");
		}
	}
	*/

	/*OXID TEST
	OXIDScanner scanner = OXIDScanner();
	threadList.push_back(thread(threadCheckFunc, scanner, "192.168.4.11"));*/

	/*SMB TEST
	SMBScanner scanner = SMBScanner();
	threadList.push_back(thread(threadPTKFunc, scanner, "192.168.4.11"));*/

	/*WMI TEST
	WMIScanner scanner = WMIScanner();
	threadList.push_back(thread(threadPthFunc, scanner, "192.168.4.11"));
	*/

	/*LOGGER SINGLE TEST
	m_logger* m1 = m_logger::getLogger();
	m_logger* m2 = m_logger::getLogger();
	m_logger* m3 = m_logger::getLogger();
	m_logger* m4 = m_logger::getLogger();
	printf("%x\n%x\n%x\n%x\n", m1, m2, m3, m4);
	*/
	
	//	LDAP_API ldapTest("administrator", "admin@123","192.168.4.11", "hengge.com");
	//LDAP_API ldapTest("192.168.4.11", "hengge.com");
	//	ldapTest.updateResourceBasedConstrainedDelegation("test333");
	//	ldapTest.searchDomainFileServer();
	//	ldapTest.searchGPO();
	//	ldapTest.searchDnsRecord();
	//	ldapTest.searchTrustDomain();
	
	/*
	WNET_API a;
	a.enumGroupMembers(L"win-ske-pc");
	cout << "=================== vServerLocalMemberAccount result ===================" << endl;
	for_each(begin(vServerLocalMemberAccount), end(vServerLocalMemberAccount), [&](string memberAccount)->void{
		cout << memberAccount << endl;
	});*/

	return 0;
}
