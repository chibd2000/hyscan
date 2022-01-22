#include "m_ftp_exp.h"

extern mutex g_vServiceMutex;
extern vector<ServiceVuln> g_vServiceVuln;
m_ftp_exp::m_ftp_exp(PortService portService)
{
	this->portService = portService;
}


m_ftp_exp::~m_ftp_exp()
{
	
}

void m_ftp_exp::checkServiceIsVuln(){
	if (!s_net_scanner::checkAliveReturn(this->portService.serviceIpAddr)){
		this->checkWeakpass();
	}
}

void m_ftp_exp::checkWeakpass(){
	string bugName = "weakpass";
	vector<string> vUsername{ "anonymous", "ftpuser", "ftp" };
	vector<string> vPassword{ "ftpuser", "ftp", "ftp@123", "123456", "admin@123"};
	SOCKET clientSocket;
	TcpClient tcpClient;
	char szBuffer[100]{ 0 };

	int packetSize;
	string receiveData1;
	
	if (!tcpClient.initWinSock())
	{
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, this->portService.serviceIpAddr, this->portService.dwServicePort))
			{
				for (DWORD i = 0;i < vUsername.size();i++){
					for (DWORD j = 0; j < vPassword.size(); j++){
						//cout << vUsername[i] << " " << vPassword[j];
						memset(szBuffer, 0, 100);
						sprintf(szBuffer, "USER %s\r\n", vUsername[i].data());
						if (!tcpClient.sendData(clientSocket, string(szBuffer)))
						{
							memset(szBuffer, 0, 100);
							sprintf(szBuffer, "PASS %s\r\n", vPassword[j].data());
							if (!tcpClient.sendData(clientSocket, string(szBuffer)))
							{
								Sleep(50);
								if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
								{
									if (receiveData1.find("230") != string::npos){
										memset(szBuffer, 0, 100);
										sprintf(szBuffer, "%s %s %s %s/%s", this->portService.serviceIpAddr.data(), this->portService.serviceNameString.data(), 
											bugName.data(), vUsername[i].data(), vPassword[j].data());
										g_vServiceMutex.lock();
										g_vServiceVuln.push_back(ServiceVuln(this->portService.serviceNameString, true, true, this->portService.dwServicePort, szBuffer));
										g_vServiceMutex.unlock();
										closesocket(clientSocket);
										return;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	closesocket(clientSocket);
}
