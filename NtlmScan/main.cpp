#define _CRT_SECURE_NO_WARNINGS
#include "public.h"
#include "TcpClient.h"
#include "NtlmInfo.h"
#include "NtlmParser.h"
#include "WMIScanner.h"
#include "SMBScanner.h"
#include "Tools.h"

static unsigned char g_szBuffer[5];

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
	Sleep(0);
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

int main(int argc, char* argv[]){	
	/* write in 2021.12.09 15.43 @zpchcbd for WMINtlmScan study*/
	
	vector<thread> threadList = vector<thread>();
	getBanner();
	// getConsole(argc, argv);
	unsigned int ip;
	unsigned int mask;
	if (argc == 4){
		if (strcmp("wmiscan", argv[1]) == 0 && strcmp("-c", argv[2]) == 0){
			sscanf(argv[3], "%hhu.%hhu.%hhu.%hhu/%hhu", &g_szBuffer[0], &g_szBuffer[1], &g_szBuffer[2], &g_szBuffer[3], &g_szBuffer[4]);
			if (g_szBuffer[4] > 32) 
				return -1;
			ip = (g_szBuffer[0] << 24UL) | (g_szBuffer[1] << 16UL) | (g_szBuffer[2] << 8UL) | (g_szBuffer[3]);
			mask = (0xFFFFFFFFUL << (32 - g_szBuffer[4])) & 0xFFFFFFFFUL;
			unsigned int startIp = ip & mask;
			unsigned int lastIp = startIp | ~mask; //   111 1111 1111 0000  |  0000 0000 0000 1111	
			WMIScanner scanner = WMIScanner();
			for (unsigned int i = startIp; i <= lastIp; i++)
				threadList.push_back(thread(threadFunc, scanner, int2ip(i)));
			for (unsigned int i = 0; i < threadList.size(); i++)
				threadList[i].join();
		}
		else if (strcmp("-c", argv[2]) == 0 && strcmp("oxidscan", argv[1]) == 0)
		{
			sscanf(argv[3], "%hhu.%hhu.%hhu.%hhu/%hhu", &g_szBuffer[0], &g_szBuffer[1], &g_szBuffer[2], &g_szBuffer[3], &g_szBuffer[4]);
			if (g_szBuffer[4] > 32)
				return -1;
			ip = (g_szBuffer[0] << 24UL) | (g_szBuffer[1] << 16UL) | (g_szBuffer[2] << 8UL) | (g_szBuffer[3]);
			mask = (0xFFFFFFFFUL << (32 - g_szBuffer[4])) & 0xFFFFFFFFUL;
			unsigned int startIp = ip & mask;
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