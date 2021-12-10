#define _CRT_SECURE_NO_WARNINGS
#include "public.h"
#include "TcpClient.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"
#include "WMIScanner.h"
#include "SMBScanner.h"
#include "Tools.h"

void getBanner(){
	cout << "[+] Welcome from HengGe Team :)" << endl;
}

void getConsole(int argc, char* argv[]){
	// console https://github.com/tanakh/cmdline/issues/6
	cout << "[+] getConsole..." << endl;
}

void threadFunc(BaseScanner& scanner, string ipAddr)
{
	scanner.scan(ipAddr);
}

void getResult(){
	for (unsigned int i = 0; i < vNtlmInfo.size(); i++)
	{
		wprintf(L"[+] MACHINE_NAME: %s\n", vNtlmInfo[i].MACHINE_NAME);
		wprintf(L"\tNETBIOS_COMPUTER_NAME[0]: %s\n", vNtlmInfo[i].NETBIOS_ATTR[0].NETBIOS_COMPUTER_NAME);
		wprintf(L"\tNETBIOS_COMPUTER_NAME[1]: %s\n", vNtlmInfo[i].NETBIOS_ATTR[1].NETBIOS_COMPUTER_NAME);
		wprintf(L"\tDNS_COMPUTER_NAME[0]: %s\n", vNtlmInfo[i].DNS_ATTR[0].DNS_COMPUTER_NAME);
		wprintf(L"\tDNS_COMPUTER_NAME[1]: %s\n", vNtlmInfo[i].DNS_ATTR[1].DNS_COMPUTER_NAME);
	}
}

static unsigned char b[5];
int main(int argc, char* argv[]){
	/* write in 2021.12.09 15.43 @zpchcbd for WMINtlmScan study*/
	vector<thread> threadList = vector<thread>();
	getBanner();
	// getConsole(argc, argv);
	unsigned int ip;
	unsigned int mask;
	if (argc == 3){
		if (strcmp("-c", argv[1]) == 0){
			sscanf(argv[2], "%hhu.%hhu.%hhu.%hhu/%hhu", &b[0], &b[1], &b[2], &b[3], &b[4]);
			if (b[4] > 32) {
				return -1;
			}
			ip = (b[0] << 24UL) | (b[1] << 16UL) | (b[2] << 8UL) | (b[3]);
			mask = (0xFFFFFFFFUL << (32 - b[4])) & 0xFFFFFFFFUL;
			unsigned int startIp = ip & mask;;
			unsigned int lastIp = startIp | ~mask; //   111 1111 1111 0000  |  0000 0000 0000 1111	
			WMIScanner scanner = WMIScanner();
			for (unsigned int i = startIp; i <= lastIp; i++)
				threadList.push_back(thread(threadFunc, scanner, int2ip(i)));
			for (unsigned int i = 0; i < threadList.size(); i++)
				threadList[i].join();
		}
	}
	getResult();
	return 0;
}