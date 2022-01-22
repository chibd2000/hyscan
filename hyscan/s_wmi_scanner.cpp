#include "s_wmi_scanner.h"

const TCHAR NTLMSSP_NEGOTIATE[] = { 5, 0, 11, 3, 16, 0, 0, 0, 120, 0, 40, 0, 3, 0, 0, 0, 184, 16, 184, 16, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 160, 1, 0, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 0, 4, 93, 136, 138, 235, 28, 201, 17, 159, 232, 8, 0, 43, 16, 72, 96, 2, 0, 0, 0, 10, 2, 0, 0, 0, 0, 0, 0, 78, 84, 76, 77, 83, 83, 80, 0, 1, 0, 0, 0, 7, 130, 8, 162, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 1, 177, 29, 0, 0, 0, 15 };
extern vector<NtlmInfo> g_vNtlmInfo;
extern mutex g_vMutex;

WMIScanner::WMIScanner()
{
	
}


WMIScanner::~WMIScanner()
{	
	
}

void WMIScanner::check(string& ipAddr){
	SOCKET clientSocket;
	TcpClient tcpClient;
	string receiveData;
	int packetSize;
	if (!tcpClient.initWinSock())
	{
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, ipAddr.c_str(), WMIPORT))
			{
				if (!tcpClient.sendData(clientSocket, string((char*)NTLMSSP_NEGOTIATE, sizeof(NTLMSSP_NEGOTIATE) / sizeof(NTLMSSP_NEGOTIATE[0]))))
				{
					if (!tcpClient.receiveData(clientSocket, receiveData, &packetSize))
					{
						NtlmInfo ntlmInfo;
						RtlZeroMemory(&ntlmInfo, sizeof(NtlmInfo));
						memcpy(ntlmInfo.ipAddr, ipAddr.c_str(), 0x10); // 111.111.111.111
						NtlmParser::parser(receiveData, &ntlmInfo, BASE_WMI_NTLMSSP);
						closesocket(clientSocket);
						g_vMutex.lock();
						g_vNtlmInfo.push_back(ntlmInfo);
						g_vMutex.unlock();
					}
				}
			}
		}
	}
}

void WMIScanner::pth(string& ipAddr){
	WMIAPI wmiApi;
	wmiApi.connect(ipAddr);
	wmiApi.release();
}
