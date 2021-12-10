#include "WMIScanner.h"
#include "public.h"

const char NTLMSSP_NEGOTIATE[] = { 5, 0, 11, 3, 16, 0, 0, 0, 120, 0, 40, 0, 3, 0, 0, 0, 184, 16, 184, 16, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 160, 1, 0, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 0, 4, 93, 136, 138, 235, 28, 201, 17, 159, 232, 8, 0, 43, 16, 72, 96, 2, 0, 0, 0, 10, 2, 0, 0, 0, 0, 0, 0, 78, 84, 76, 77, 83, 83, 80, 0, 1, 0, 0, 0, 7, 130, 8, 162, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 1, 177, 29, 0, 0, 0, 15 };

WMIScanner::WMIScanner()
{

}


WMIScanner::~WMIScanner()
{	

}

void WMIScanner::scan(string ipAddr){
	SOCKET clientSocket;
	TcpClient tcpClient = TcpClient();
	string receiveData;
	tcpClient.initWinSock();
	tcpClient.createSocket(clientSocket);
	tcpClient.connectSocket(clientSocket, ipAddr.c_str(), WMIPORT);
	int t = tcpClient.sendData(clientSocket, string((char*)NTLMSSP_NEGOTIATE, sizeof(NTLMSSP_NEGOTIATE) / sizeof(NTLMSSP_NEGOTIATE[0])));
	if (t == -1)
		return;
	tcpClient.receiveData(clientSocket, receiveData);
	NtlmInfo ntlmInfo;
	ZeroMemory(&ntlmInfo, sizeof(NtlmInfo));
	NtlmParser::parser(receiveData, &ntlmInfo);
	vNtlmInfo.push_back(ntlmInfo);
}