#ifndef TCPCLIENT_H_H
#define TCPCLIENT_H_H
#include "public.h"
#define TIMEOUT 5

class TcpClient
{
public:
	TcpClient();
	~TcpClient();
	int initWinSock();
	int createSocket(SOCKET &listenerSocket);
	int connectSocket(SOCKET &conSock, std::string ip, int port);
	int sendData(SOCKET &clientSock, std::string &packetData);
	int receiveData(SOCKET &clientSock, std::string &packetData, int* packetSize);
private:
	int socketTimeOut;
};
#endif
