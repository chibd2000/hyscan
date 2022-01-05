#include "public.h"
#include "Tools.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"
#include "WMIScanner.h"
#include "SMBScanner.h"
#include "WinrmScanner.h"
#include "OXIDScanner.h"
#include "m_ldap_api.h"
#include "ScannerFactory.h"
// #include "ThreadPool.h"

static unsigned char g_szBuffer[5];
vector<NtlmInfo> vNtlmInfo;

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


void getBanner(){
	cout << "[+] Welcome from HengGe Team :)" << endl;
}

void getConsole(int argc, char* argv[]){
	// console https://github.com/tanakh/cmdline/issues/6
	cout << "[+] getConsole..." << endl;
}

void initThread(){
	// ThreadPool threadPool(20);
}
 
void threadCheckFunc(BaseScanner* scanner, string ipAddr)
{
	scanner->check(ipAddr);
	Sleep(0);
}

void threadPthFunc(BaseScanner* scanner, string ipAddr)
{
	scanner->pth(ipAddr);
	Sleep(0);
}

void getResult(){
	for (unsigned int i = 0; i < vNtlmInfo.size(); i++)
	{
		printf("[+] %s\n", vNtlmInfo[i].ipAddr);
		wprintf(L"\tMACHINE_NAME: %s\n", vNtlmInfo[i].MACHINE_NAME);
		wprintf(L"\tNETBIOS_COMPUTER_NAME[0]: %s\n", vNtlmInfo[i].NETBIOS_ATTR[0].NETBIOS_COMPUTER_NAME);
		wprintf(L"\tNETBIOS_COMPUTER_NAME[1]: %s\n", vNtlmInfo[i].NETBIOS_ATTR[1].NETBIOS_COMPUTER_NAME);
		wprintf(L"\tDNS_COMPUTER_NAME[0]: %s\n", vNtlmInfo[i].DNS_ATTR[0].DNS_COMPUTER_NAME);
		wprintf(L"\tDNS_COMPUTER_NAME[1]: %s\n", vNtlmInfo[i].DNS_ATTR[1].DNS_COMPUTER_NAME);
	}
}

int main(int argc, char* argv[]){	
	/* write in 2021.12.09 15.43 @zpchcbd for WMINtlmScan study*/
	
	vector<thread> threadList = vector<thread>();
	unsigned int ip = 0;
	unsigned int mask = 0;
	unsigned int startIp = 0;
	unsigned int lastIp = 0;
	
	getBanner();
	// getConsole(argc, argv);
	/*
	try{
		if (argv[3]){
		sscanf(argv[3], "%hhu.%hhu.%hhu.%hhu/%hhu", &g_szBuffer[0], &g_szBuffer[1], &g_szBuffer[2], &g_szBuffer[3], &g_szBuffer[4]);
		if (g_szBuffer[4] > 32) return -1;
		ip = (g_szBuffer[0] << 24UL) | (g_szBuffer[1] << 16UL) | (g_szBuffer[2] << 8UL) | (g_szBuffer[3]);
		//
		mask = (0xFFFFFFFFUL << (32 - g_szBuffer[4])) & 0xFFFFFFFFUL;
		startIp = ip & mask;
		lastIp = startIp | ~mask; //   111 1111 1111 0000  |  0000 0000 0000 1111
		}
	}catch (out_of_range & e){
		cout << e.what() << endl;
	}*/

	
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

	getResult();

	/*OXID TEST
	OXIDScanner scanner = OXIDScanner();
	threadList.push_back(thread(threadCheckFunc, scanner, "192.168.4.11"));*/

	/*SMB TEST
	SMBScanner scanner = SMBScanner();
	threadList.push_back(thread(threadPTKFunc, scanner, "192.168.4.11"));*/

	/*WMI TEST
	WMIScanner scanner = WMIScanner();
	threadList.push_back(thread(threadPthFunc, scanner, "192.168.4.11"));
	threadList[0].join();
	*/

	/*LOGGER SINGLE TEST
	m_logger* m1 = m_logger::getLogger();
	m_logger* m2 = m_logger::getLogger();
	m_logger* m3 = m_logger::getLogger();
	m_logger* m4 = m_logger::getLogger();
	printf("%x\n%x\n%x\n%x\n", m1, m2, m3, m4);
	*/
	
	/*
	LDAP_API ldapTest("administrator", "admin@123", "192.168.4.11", "hengge.com");
	ldapTest.searchConstrainedDelegation();*/
	
	

	return 0;
}