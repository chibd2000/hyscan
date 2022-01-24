#include "m_redis_exp.h"

extern mutex g_vServiceMutex;
extern vector<ServiceVuln> g_vServiceVuln;

m_redis_exp::m_redis_exp(PortService portService)
{
	this->portService = portService;
}


m_redis_exp::~m_redis_exp()
{
}

void m_redis_exp::checkServiceIsVuln(){
	if (!checkAliveReturn(this->portService.serviceIpAddr)){
		this->checkWeakpass();
		this->checkUnauth();
	}
}


void m_redis_exp::checkUnauth(){
	string bugName = "unauth";
	SOCKET clientSocket;
	TcpClient tcpClient;
	string payload = "INFO\n";
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
						if (regex_search(receiveData1, regex("redis_version"))){
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
			else{
				return;
			}
		}
	}
	closesocket(clientSocket);
}

void m_redis_exp::checkWeakpass(){
	string bugName = "weakpass";
	string payload = "INFO\n";

	vector<string> vPassword{ "Aa@123456", "123456", "redis@123", "admin@123", "root", "admin", "default", "redis", "root123" };

	SOCKET clientSocket;
	TcpClient tcpClient;
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
					Sleep(50);
					if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
					{
						if (receiveData1.find("Authentication") != string::npos){
							for (DWORD i = 0; i < vPassword.size(); i++){
								receiveData1.clear();
								memset(szBuffer, 0, 100);
								sprintf(szBuffer, "AUTH %s\r\n", vPassword[i].data());
								if (!tcpClient.sendData(clientSocket, string(szBuffer)))
								{	
									Sleep(50);
									if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
									{
										if (receiveData1.find("+OK") != string::npos){
											memset(szBuffer, 0, 100);
											sprintf(szBuffer, "%s %s %s %s", this->portService.serviceIpAddr.data(), this->portService.serviceNameString.data(),
												bugName.data(), vPassword[i].data());
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
			else{
				return;
			}
		}
	}
	closesocket(clientSocket);
}