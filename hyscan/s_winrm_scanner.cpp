#include "s_winrm_scanner.h"

WinrmScanner::WinrmScanner()
{
}


WinrmScanner::~WinrmScanner()
{
}

void WinrmScanner::check(string& ipAddr){
	/*
	HttpClient httpClient = HttpClient();
	string receiveData;
	if (!tcpClient.initWinSock())
	{
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, ipAddr.c_str(), WMIPORT))
			{
				if (!tcpClient.sendData(clientSocket, string((char*)NTLMSSP_NEGOTIATE, sizeof(NTLMSSP_NEGOTIATE) / sizeof(NTLMSSP_NEGOTIATE[0]))))
				{
					if (!tcpClient.receiveData(clientSocket, receiveData))
					{
						NtlmInfo ntlmInfo;
						ZeroMemory(&ntlmInfo, sizeof(NtlmInfo));
						memcpy(ntlmInfo.ipAddr, ipAddr.c_str(), 0x10); // 111.111.111.111
						NtlmParser::parser(receiveData, &ntlmInfo, BASE_WMI_NTLMSSP);
						closesocket(clientSocket);
						vNtlmInfo.push_back(ntlmInfo);
					}
				}
			}
		}
	}*/
}

void WinrmScanner::pth(string& ipAddr){

}
