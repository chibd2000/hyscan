#include "m_zookeeper_exp.h"
extern mutex g_vServiceMutex; 
extern vector<ServiceVuln> g_vServiceVuln;

m_zookeeper_exp::m_zookeeper_exp(PortService portService)
{
	this->portService = portService;
}


m_zookeeper_exp::~m_zookeeper_exp()
{

}

void m_zookeeper_exp::checkServiceIsVuln(){
	if (!checkAliveReturn(this->portService.serviceIpAddr)){
		this->checkUnauth();
	}
}

void m_zookeeper_exp::checkUnauth(){
	string bugName = "unauth";
	SOCKET clientSocket;
	TcpClient tcpClient;
	string payload = "envi\n";
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
						if (receiveData1.find("zookeeper version") != string::npos){
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
	}
	closesocket(clientSocket);
}
