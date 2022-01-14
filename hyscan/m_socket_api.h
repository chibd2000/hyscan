#ifndef TCPCLIENT_H_H
#define TCPCLIENT_H_H
#include "public.h"
#define TIMEOUT 5

class TcpClient
{
public:
	TcpClient();
	~TcpClient();
	DWORD initWinSock();
	DWORD createSocket(SOCKET& listenerSocket);
	DWORD connectSocket(SOCKET& conSock, string ip, int port);
	DWORD sendData(SOCKET& clientSock, string& packetData);
	DWORD receiveData(SOCKET& clientSock, string& packetData, int* packetSize);
private:
	DWORD socketTimeOut;
};
#endif
