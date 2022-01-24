#include "m_rsync_exp.h"

extern mutex g_vServiceMutex;
extern vector<ServiceVuln> g_vServiceVuln;

m_rsync_exp::m_rsync_exp(PortService portService)
{
	this->portService = portService;
}


m_rsync_exp::~m_rsync_exp()
{

}

void m_rsync_exp::checkServiceIsVuln(){
	if (!checkAliveReturn(this->portService.serviceIpAddr)){
		this->checkUnauth();
	}
}

void m_rsync_exp::checkUnauth(){
	string bugName = "unauth";
	SOCKET clientSocket;
	TcpClient tcpClient;
	string payload = "@RSYNCD: 31\n";
	char szBuffer[100] = { 0 };
	int packetSize;
	string receiveData1;
	if (!tcpClient.initWinSock())
	{
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, this->portService.serviceIpAddr, this->portService.dwServicePort))
			{
				if (!tcpClient.sendData(clientSocket, payload))
				{
					if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
					{
						vector<string> vString = split(receiveData1, "\n");
						for (DWORD i = 0;i<vString.size();i++){
							if (vString[i].find("@RSYNCD: EXIT") == string::npos && !vString[i].empty()){
								memset(szBuffer, 0, 100);
								sprintf(szBuffer, "%s %s %s", this->portService.serviceIpAddr.data(), this->portService.serviceNameString.data(), bugName.data());
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
			else{
				return;
			}
		}
	}
	closesocket(clientSocket);
}