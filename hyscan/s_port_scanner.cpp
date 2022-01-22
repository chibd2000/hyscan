#include "s_port_scanner.h"

extern mutex g_vServiceMutex;
extern vector<PortService> g_vAliveIpPort;
extern vector<PortService> g_vVulnServiceIpPort;
extern neb::CJsonObject g_portServiceJson;
extern neb::CJsonObject g_portServiceBannerJson;

PortScanner::PortScanner()
{
	
}


PortScanner::~PortScanner()
{

}

void PortScanner::serviceCheck(string& ipAddr, string& port){
	ServiceScanner serviceScanner;
	DWORD dwPort = atoi(port.c_str());
	serviceScanner.serviceCheck(ipAddr, dwPort, "tcp");
}

void ServiceScanner::serviceExploit(PortService portService){
	if (portService.serviceNameString == "dubbo"){
		m_dubbo_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "ftp"){
		m_ftp_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "javarmi"){
		m_javarmi_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "ldap"){
		m_ldap_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "mongodb"){
		m_mongodb_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "mysql"){
		m_mysql_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "mssql"){
		m_mssql_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "memcache"){
		m_memcache_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "redis"){
		m_redis_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "rsync"){
		m_rsync_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "smb"){
		m_smb_exp(portService).checkServiceIsVuln();
	}
	else if (portService.serviceNameString == "zookeeper"){
		m_zookeeper_exp(portService).checkServiceIsVuln();
	}
}

void ServiceScanner::serviceCheck(string ipAddr, DWORD dwPort, string scanType){
	if (scanType == "tcp"){
		this->tcpServiceCheck(ipAddr, dwPort);
	}
	else if (scanType == "udp"){
		this->udpServiceCheck(ipAddr, dwPort);
	}
	else{
		this->tcpServiceCheck(ipAddr, dwPort);
	}
}

void ServiceScanner::udpServiceCheck(string ipAddr, DWORD dwPort){

}

void ServiceScanner::tcpServiceCheck(string ipAddr, DWORD dwPort){
	//crypto_Base64 b64;
	//std::string str2 = "ClsgIAogICAgewogICAgICAgICJuYW1lIjogInZuYyIsCiAgICAgICAgInByb2Jlc3RyaW5nIjogIiIsCiAgICAgICAgInBhdHRlcm4iOiBbIl5SRkIgMDAiXSwKICAgICAgICAicG9ydCI6IFsiNTkwMSJdCiAgICB9LCAKICAgIHsKICAgICAgICAibmFtZSI6ICJUZXJtaW5hbCBTZXJ2ZXIiLAogICAgICAgICJwcm9iZXN0cmluZyI6ICJcXHgwM1xceDAwXFx4MDBcXHgwYlxceDA2XFx4ZTBcXHgwMFxceDAwXFx4MDBcXHgwMFxceDAwIiwKICAgICAgICAicGF0dGVybiI6IFsiXlxceDAzXFx4MDBcXHgwMCJdLAogICAgICAgICJwb3J0IjogWyIzMzg5IiwgIjMzODgiLCAiMzM4OTkiXQogICAgfSwKCXsKCQkibmFtZSI6ICJtc3NxbCIsCgkJInByb2Jlc3RyaW5nIjogIlxceDEyXFx4MDFcXHgwMFxceDM0XFx4MDBcXHgwMFxceDAwXFx4MDBcXHgwMFxceDAwXFx4MTVcXHgwMFxceDA2XFx4MDFcXHgwMFxceDFiXFx4MDBcXHgwMVxceDAyXFx4MDBcXHgxY1xceDAwXFx4MGNcXHgwM1xceDAwXFx4MjhcXHgwMFxceDA0XFx4ZmZcXHgwOFxceDAwXFx4MDFcXHg1NVxceDAwXFx4MDBcXHgwMFxceDRkXFx4NTNcXHg1M1xceDUxXFx4NGNcXHg1M1xceDY1XFx4NzJcXHg3NlxceDY1XFx4NzJcXHgwMFxceDQ4XFx4MGZcXHgwMFxceDAwIiwKCQkicGF0dGVybiI6IFsiTVNTUUxTZXJ2ZXIiLCAiXlxceDA0XFx4MDFcXHgwMFxceDI1XFx4MDBcXHgwMFxceDAxXFx4MDBcXHgwMFxceDAwXFx4MTUiXSwKCQkicG9ydCI6IFsiMTQzMyJdCiAgICB9LAogICAgewogICAgICAgICJuYW1lIjogIm15c3FsIiwKICAgICAgICAicHJvYmVzdHJpbmciOiAiIiwKICAgICAgICAicGF0dGVybiI6IFsiXi4qXFx4MDBcXHgwMFxceDAwXFx4MGEiLCAiXFx4NGFcXHgwMFxceDAwXFx4MDBcXHgwYVxceDM1IiwgIm15c3FsX25hdGl2ZV9wYXNzd29yZCJdLAogICAgICAgICJwb3J0IjogWyIzMzA2Il0KICAgIH0sCiAgICB7CiAgICAgICAgIm5hbWUiOiAicG9zdGdyZXNxbCIsCiAgICAgICAgInByb2Jlc3RyaW5nIjogIiIsCiAgICAgICAgInBhdHRlcm4iOiBbImludmFsaWQgcGFja2V0IGxlbmd0aCIsICJeZWZhdGFsIl0sCiAgICAgICAgInBvcnQiOiBbIjExMTEiXQogICAgfSwKICAgIHsKICAgICAgICAibmFtZSI6ICJzc2giLAogICAgICAgICJwcm9iZXN0cmluZyI6ICIiLAogICAgICAgICJwYXR0ZXJuIjogWyJeU1NILS4qb3BlbnNzaCJdLAogICAgICAgICJwb3J0IjogWyIyMiJdCiAgICB9LAogICAgewogICAgICAgICJuYW1lIjogIkphdmEtUk1JIiwKICAgICAgICAicHJvYmVzdHJpbmciOiAiXFx4NGFcXHg1MlxceDRkXFx4NDlcXHgwMFxceDAyXFx4NGIiLAogICAgICAgICJwYXR0ZXJuIjogWyJeXFx4NGUuLiJdLAogICAgICAgICJwb3J0IjogWyIxMDk5Il0KICAgIH0sCiAgICB7CiAgICAgICAgIm5hbWUiOiAiYWN0aXZlbXEiLAogICAgICAgICJwcm9iZXN0cmluZyI6ICIiLAogICAgICAgICJwYXR0ZXJuIjogWyJeRVJST1Jcbig/OlteXG5dK1xuKT9tZXNzYWdlOlVua25vd24gU1RPTVAgYWN0aW9uOi4rIG9yZ1wuYXBhY2hlXC5hY3RpdmVtcVwuIl0sCiAgICAgICAgInBvcnQiOiBbIjYxNjEzIiwgIjYxNjMiXQogICAgfSwKICAgIHsKICAgICAgICAibmFtZSI6ICJ6b29rZWVwZXIiLAogICAgICAgICJwcm9iZXN0cmluZyI6ICJzdGF0c1xyXG4iLAogICAgICAgICJwYXR0ZXJuIjogWyJeem9va2VlcGVyIHZlcnNpb246ICJdLAogICAgICAgICJwb3J0IjogWyIyMTgxIl0KICAgIH0sCiAgICB7CiAgICAgICAgIm5hbWUiOiAiZnRwIiwKICAgICAgICAicGF0dGVybiI6IFsiXjIyMC4qXG4zMzEiLCAiXjIyMC4qXG41MzAiLCAiXjIyMC4qRlRQIiwgIl4yMjAgLiogTWljcm9zb2Z0IC4qIEZUUCIsICJeMjIwIEluYWN0aXZpdHkgdGltZXIiLCAiXjIyMCAuKiBVc2VyR2F0ZSIsICJeMjIwLipGaWxlWmlsbGEgU2VydmVyIl0sCiAgICAgICAgInBvcnQiOiBbIjIxIl0KICAgIH0sCiAgICB7CiAgICAgICAgIm5hbWUiOiAibGRhcCIsCiAgICAgICAgInBhdHRlcm4iOiBbIl5cXHgzMFxceDBjXFx4MDJcXHgwMVxceDAxXFx4NjEiLCAiXlxceDMwXFx4MzJcXHgwMlxceDAxIiwgIl5cXHgzMFxceDMzXFx4MDJcXHgwMSIsICJeXFx4MzBcXHgzOFxceDAyXFx4MDEiLCAiXlxceDMwXFx4ODQiLCAiXlxceDMwXFx4NDUiXSwKICAgICAgICAicG9ydCI6IFsiMzg5Il0KICAgIH0sCiAgICB7CiAgICAgICAgIm5hbWUiOiAibW9uZ29kYiIsCiAgICAgICAgInByb2Jlc3RyaW5nIjogIlxceDQxXFx4MDBcXHgwMFxceDAwXFx4M2FcXHgzMFxceDAwXFx4MDBcXHhmZlxceGZmXFx4ZmZcXHhmZlxceGQ0XFx4MDciLAogICAgICAgICJwYXR0ZXJuIjogWyJeLip2ZXJzaW9uLi4uLi4iXSwKICAgICAgICAicG9ydCI6IFsiMjcwMTciLCAiOTAwMSIsICI0OTE1MyJdCiAgICB9LAogICAgewogICAgICAgICJuYW1lIjogImFqcDEzIiwKICAgICAgICAicHJvYmVzdHJpbmciOiAiXFx4MTJcXHgzNFxceDAwXFx4MDFcXHgwYSIsCiAgICAgICAgInBhdHRlcm4iOiBbIl5cXHg0MVxceDQyXFx4MDBcXHgwMVxceDA5JCJdLAogICAgICAgICJwb3J0IjogWyI4MDA5Il0KICAgIH0sCiAgICB7CiAgICAgICAgIm5hbWUiOiAicmVkaXMiLAogICAgICAgICJwcm9iZXN0cmluZyIgOiAiKjFcclxuJDRcclxuaW5mb1xyXG4iLAogICAgICAgICJwYXR0ZXJuIjogWyJeLUVSUiB1bmtub3duIGNvbW1hbmQiLCAiXi1FUlIgd3JvbmcgbnVtYmVyIG9mIGFyZ3VtZW50cyIsICJeLURFTklFRCBSZWRpcyBpcyBydW5uaW5nIiwgIi1FUlIgb3BlcmF0aW9uIG5vdCBwZXJtaXR0ZWQiLCAiXlxcJFxcZCtcXHJcXG4oPzojW15cXHJcXG5dKlxcclxcbikqcmVkaXNfdmVyc2lvbjooWy5cXGRdKylcXHJcXG4iXSwKICAgICAgICAicG9ydCI6IFsiNjM3OSJdCiAgICB9LAogICAgewoJCSJuYW1lIjogImR1YmJvIiwKICAgICAgICAicGF0dGVybiI6IFsiXmR1YmJvPiQiXSwKICAgICAgICAicG9ydCI6IFsiMjA4ODAiXQogICAgfSwKICAgIHsKICAgICAgICAibmFtZSI6ICJqZHdwIiwKICAgICAgICAicHJvYmVzdHJpbmciIDogIkpEV1AtSGFuZHNoYWtlIiwKICAgICAgICAicGF0dGVybiI6IFsiXkpEV1AtSGFuZHNoYWtlJCJdLAogICAgICAgICJwb3J0IjogWyI4MDAwIl0KICAgIH0sCiAgICB7CiAgICAgICAgIm5hbWUiOiAibWVtY2FjaGUiLAogICAgICAgICJwcm9iZXN0cmluZyI6ICJzdGF0c1xyXG4iLAogICAgICAgICJwYXR0ZXJuIjogWyJeU1RBVCAiXSwKICAgICAgICAicG9ydCI6IFsiMTEyMTEiXQogICAgfSwKICAgIHsKICAgICAgICAibmFtZSI6ICJyc3luYyIsCiAgICAgICAgInBhdHRlcm4iOiBbIl5AUlNZTkNEOiIsICJAUlNZTkNEOiJdLAogICAgICAgICJwb3J0IjogWyI4NzMiXQogICAgfQpdCg==";
	//std::cout << b64.Decode(str2.data(), sizeof(str2)-1);
	//neb::CJsonObject oJson(b64.Decode(str2.data(), str2.length()))
	//neb::CJsonObject oJson(portServiceBanner);
	
	//std::cout << g_portServiceBannerJson.ToString() << std::endl;
	//std::cout << "---------------------------------------------" << std::endl;
	//std::cout << g_portServiceJson.ToString() << std::endl;

	SOCKET clientSocket;
	TcpClient tcpClient;
	string openIpPortString;
	string receiveData;
	int i, j, k;
	int packetSize;
	if (!tcpClient.initWinSock())
	{
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, ipAddr, dwPort))
			{
				// 如果能连接上那么该端口则为开放状态
				std::cout << "----------------------------端口开放情况-----------------------------" << std::endl;
				openIpPortString = ipAddr + ":" + to_string(dwPort);
				cout << "open: " << openIpPortString << endl;
				PortService pppp = PortService("", ipAddr, dwPort, PORT_OPEN, SERVICE_NO_KNOWN);
				string serviceNameString = getServicebyPortList(dwPort);
				if (!serviceNameString.empty()){
					// cout << serviceNameString << endl;
					g_vServiceMutex.lock();
					// cout << serviceNameString << endl;
					pppp.serviceNameString = serviceNameString;
					pppp.dwServiceStatus = SERVICE_KNOW;
					g_vAliveIpPort.push_back(pppp);
					g_vServiceMutex.unlock();
				}
				else{
					g_vServiceMutex.lock();
					g_vAliveIpPort.push_back(pppp);
					g_vServiceMutex.unlock();
				}
				// 先关闭原来的socket
				closesocket(clientSocket);
			}else{
				// 先关闭原来的socket
				closesocket(clientSocket);
				return;
			}
		}
	}

	std::cout << "----------------------------端口服务探测1-----------------------------" << std::endl;
	for (i = 0; i < g_portServiceBannerJson.GetArraySize(); i++)
	{
		for (j = 0; j < g_portServiceBannerJson[i]["port"].GetArraySize(); j++)
		{
			// 如果端口开放，优先尝试默认端口 
			if (to_string(dwPort) == g_portServiceBannerJson[i]["port"](j))
			{
				if (!tcpClient.createSocket(clientSocket))
				{
					if (!tcpClient.connectSocket(clientSocket, ipAddr, dwPort))
					{
						// 如果优先默认端口有匹配到，探测下面就来匹配banner
						if (!tcpClient.sendData(clientSocket, getHexString(g_portServiceBannerJson[i]("probestring"))))
						{
							Sleep(50);
							if (!tcpClient.receiveData(clientSocket, receiveData, &packetSize)){
								for (k = 0; k<g_portServiceBannerJson[i]["pattern"].GetArraySize(); k++)
								{
									if (regex_search(receiveData, regex(g_portServiceBannerJson[i]["pattern"](k))))
									{
										PortService portService = PortService(g_portServiceBannerJson[i]("name"), ipAddr, dwPort, PORT_OPEN, SERVICE_KNOW);
										g_vServiceMutex.lock();
										g_vVulnServiceIpPort.push_back(portService);
										g_vServiceMutex.unlock();
										std::cout << g_portServiceBannerJson[i]("name") << " " << openIpPortString << " ";
										cout << "banner ----> " << g_portServiceBannerJson[i]["pattern"](k) << endl;
										closesocket(clientSocket);
										// 那么继续进行利用探测
										this->serviceExploit(portService);
										return;
									}
								}
								receiveData.clear();
							}
						}
						else{
							// 如果优先默认端口有匹配到，但是发送数据直接断开的情况
							// 一般来说如果发送数据都直接失败的话，那么可能的情况就是该服务默认就是不允许外网连接的，所以这种就不需要继续往下面进行探测了
							// 如果此时 当前端口存在默认端口列表的话，不管是不是真的对应的端口服务，默认就对应端口的这个服务即可
							g_vVulnServiceIpPort.push_back(PortService(g_portServiceBannerJson[i]("name"), ipAddr, dwPort, PORT_OPEN, SERVICE_UNKNOWN));
							closesocket(clientSocket);
							return;
						}
					}
				}
				closesocket(clientSocket);
			}
		}
	}

	// 优先默认端口有匹配到， 发送数据 并且 能接收回数据的情况
	// 但是没有匹配到 默认端口服务的指纹
	// 这种情况就是虽然是默认端口匹配，但是该服务不是默认端口对应的服务
	// 所以这里的话还需要继续探测 进行第二波探测
	// 这里优先探测
	cout << "----------------------------端口服务探测2-----------------------------" << endl;
	for (i = 0; i < g_portServiceBannerJson.GetArraySize(); i++){
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, ipAddr, dwPort))
			{
				if (!tcpClient.sendData(clientSocket, getHexString(g_portServiceBannerJson[i]("probestring"))))
				{
					Sleep(50);
					if (!tcpClient.receiveData(clientSocket, receiveData, &packetSize))
					{
						for (k = 0; k < g_portServiceBannerJson[i]["pattern"].GetArraySize(); k++)
						{
							if (regex_search(receiveData, regex(g_portServiceBannerJson[i]["pattern"](k))))
							{
								PortService portService = PortService(g_portServiceBannerJson[i]("name"), ipAddr, dwPort, PORT_OPEN, SERVICE_KNOW);
								g_vServiceMutex.lock();
								g_vVulnServiceIpPort.push_back(portService);
								g_vServiceMutex.unlock();
								cout << g_portServiceBannerJson[i]("name") << " " << openIpPortString << " ";
								cout << "banner ----> " << g_portServiceBannerJson[i]["pattern"](k) << endl;
								// 找到了就直接return了
								closesocket(clientSocket);
								// 那么继续进行利用探测
								this->serviceExploit(portService);
								return;
							}
						}
						receiveData.clear();
					}
				}
			}
		}
		closesocket(clientSocket);
	}
	/*
	// 如果端口开放，优先尝试默认端口对应的匹配banner发现没有匹配到
	// 这时候有可能是不是默认端口开放，所以需要遍历常见的banner来进行匹配
	std::cout << "----------------------------端口服务探测3-----------------------------" << std::endl;
	string serviceNameString = getServicebyPortList(dwPort);
	if (!serviceNameString.empty()){
		// cout << serviceNameString << endl;
		g_vServiceMutex.lock();
		g_vAliveIpPort.push_back(PortService(serviceNameString, ipAddr, dwPort, PORT_OPEN, SERVICE_KNOW));
		g_vServiceMutex.unlock();

	}*/
}



string ServiceScanner::getServicebyPortList(DWORD dwPort){
	string keyString;
	DWORD isGet = 0;
	int i;
	// cout << g_portServiceJson.GetArraySize() << endl;
	for (i = 0; i<g_portServiceJson.GetArraySize(); i++){
		if (g_portServiceJson[i].GetKey(keyString) && keyString == to_string(dwPort)){
			isGet = 1;
			break;
		}
	}
	if (isGet){
		return g_portServiceJson[i](to_string(dwPort));
	}
	else{
		return "";
	}
}