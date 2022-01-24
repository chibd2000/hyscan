#include "m_mongodb_exp.h"

extern mutex g_vServiceMutex;
extern vector<ServiceVuln> g_vServiceVuln;

m_mongodb_exp::m_mongodb_exp(PortService portService)
{
	this->portService = portService;
}

m_mongodb_exp::~m_mongodb_exp()
{
}

void m_mongodb_exp::checkServiceIsVuln(){
	if (!checkAliveReturn(this->portService.serviceIpAddr)){
		this->checkUnauth();
	}
}

void m_mongodb_exp::checkUnauth(){
	string bugName = "unauth";
	SOCKET clientSocket;
	TcpClient tcpClient;
	string payload1 = R"(\x3a\x00\x00\x00\xa7\x41\x00\x00\x00\x00\x00\x00\xd4\x07\x00\x00\x00\x00\x00\x00\x61\x64\x6d\x69\x6e\x2e\x24\x63\x6d\x64\x00\x00\x00\x00\x00\xff\xff\xff\xff\x13\x00\x00\x00\x10\x69\x73\x6d\x61\x73\x74\x65\x72\x00\x01\x00\x00\x00\x00)";
	string payload2 = R"(\x48\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\xd4\x07\x00\x00\x00\x00\x00\x00\x61\x64\x6d\x69\x6e\x2e\x24\x63\x6d\x64\x00\x00\x00\x00\x00\x01\x00\x00\x00\x21\x00\x00\x00\x02\x67\x65\x74\x4c\x6f\x67\x00\x10\x00\x00\x00\x73\x74\x61\x72\x74\x75\x70\x57\x61\x72\x6e\x69\x6e\x67\x73\x00\x00)";
	
	char szBuffer[100] = { 0 };
	int packetSize;
	string receiveData1;
	if (!tcpClient.initWinSock())
	{
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, this->portService.serviceIpAddr, this->portService.dwServicePort))
			{
				if (!tcpClient.sendData(clientSocket, getHexString(payload1)))
				{
					Sleep(50);
					if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
					{
						if (receiveData1.find("ismaster") != string::npos){
							receiveData1.clear();
							if (!tcpClient.sendData(clientSocket, getHexString(payload2)))
							{
								Sleep(50);
								if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
								{
									if (receiveData1.find("totalLinesWritten") != string::npos){
										memset(szBuffer, 0, 100);
										sprintf(szBuffer, "%s %s %s", this->portService.serviceIpAddr.data(), this->portService.serviceNameString.data(), bugName.data());
										g_vServiceMutex.lock();
										g_vServiceVuln.push_back(ServiceVuln("mongodb", true, true, this->portService.dwServicePort, szBuffer));
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