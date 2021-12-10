#include "TcpClient.h"


TcpClient::TcpClient()
{
	socketTimeOut = 3000;
}


TcpClient::~TcpClient()
{
}

/*
初始化winSock
*/
int TcpClient::initWinSock()
{
	WORD verision = MAKEWORD(2, 2);
	WSADATA lpData;
	int iFlag = WSAStartup(verision, &lpData); // 指定winsock版本并初始化
	if (iFlag != 0)
	{
#ifdef DEBUG
		std::cout << "[-] winSock init fail" << std::endl;
#else
		/*null*/
#endif
		return -1;
	}
#ifdef DEBUG
	std::cout << "[+] winSock init success" << std::endl;
#else
	/*null*/
#endif
	return 0;
}

/*
创建侦听socket  
*/
int TcpClient::createSocket(SOCKET &clientSocket)
{
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&socketTimeOut, sizeof(int));
	if (clientSocket == INVALID_SOCKET)
	{
#ifdef DEBUG
		cout << "[-] socket create fail" << endl;
#else
		/*null*/
#endif
		return -1;
	}

#ifdef DEBUG
	cout << "[+] socket create success" << endl;
#else
	/*null*/
#endif
	return 0;
}

/*
连接服务器
*/
int TcpClient::connectSocket(SOCKET &conSock, std::string ip, int port)
{
	// 建立地址结构体
	sockaddr_in hostAddr;
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_port = htons(port);//转换成网络字节序  
	//hostAddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);//转换成网络字节序  
	//cout << "net IP:" << hostAddr.sin_addr.S_un.S_addr << endl;  
	/*
	inet_addr()版本太低，被弃用使用inet_pton(协议族，字符串IP地址，void目标in_addr*)
	头文件：WS2tcpip.h
	*/
	in_addr addr;
	inet_pton(AF_INET, ip.c_str(), (void*)&addr);
	hostAddr.sin_addr = addr;
	// std::cout << "ip(网络字节序):" << addr.S_un.S_addr << std::endl;
	cout << "[+] scanning target ip =========================> " << ip.c_str() << endl;

	// 向服务器提出连接请求
	int err = connect(conSock, (sockaddr*)&hostAddr, sizeof(sockaddr));
	if (err == INVALID_SOCKET)
	{
#ifdef DEBUG
		cout << "[-] connect server fail" << endl;
#else
		/*null*/
#endif
		return -1;
	}
	return 0;
}

int TcpClient::sendData(SOCKET &clientSock, string &packetData)
{
	int err = send(clientSock, packetData.c_str(), packetData.size(), 0);
	if (err == SOCKET_ERROR)
	{
#ifdef DEBUG
		cout << "[-] send fail" << endl;
#else
		/*null*/
#endif
		return -1;
	}

#ifdef DEBUG
	cout << "[+] send data ====> " << packetData.c_str() << endl;
#else
	/*null*/
#endif
	return 0;
}

int TcpClient::receiveData(SOCKET &clientSock, string &packetData)
{
	static int cnt = 1; // 接收数据编号-静态
	// 通过已建立连接的套接字，接收数据 设定缓冲1024字节
	char buf[1024] = {0};
	// flags操作方式（0正常数据，MSG_PEED系统缓冲区的数据复制到所提供的接收缓冲区内，系统缓冲区数据未删除，MSG_OOB处理带外数据，通常用参数0即可）
	int buflen = recv(clientSock, buf, 1024, 0);
	if (buflen == SOCKET_ERROR)
	{
#ifdef DEBUG
		cout << "[-] recv data fail" << endl;
#else
		/*null*/
#endif
		return -1;
	}
	// 一切正常则显示接收数据
	packetData = string(buf, 1024);
#ifdef DEBUG
	cout << "[+] recv data ====> " << buf << endl;
#else
	/*null*/
#endif

	return 0;
}