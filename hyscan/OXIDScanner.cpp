#include "OXIDScanner.h"

const char IOXID_RPC_NEGOTIATE[] = {
	0x05, 0x00, 0x0b, 0x03, 0x10, 0x00, 0x00, 0x00,
	0x48, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0xb8, 0x10, 0xb8, 0x10, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
	0xc4, 0xfe, 0xfc, 0x99, 0x60, 0x52, 0x1b, 0x10,
	0xbb, 0xcb, 0x00, 0xaa, 0x00, 0x21, 0x34, 0x7a,
	0x00, 0x00, 0x00, 0x00, 0x04, 0x5d, 0x88, 0x8a,
	0xeb, 0x1c, 0xc9, 0x11, 0x9f, 0xe8, 0x08, 0x00,
	0x2b, 0x10, 0x48, 0x60, 0x02, 0x00, 0x00, 0x00 };

const char IOXID_RPC_REQUEST[] = {
	0x05, 0x00, 0x00, 0x03, 0x10, 0x00, 0x00, 0x00,
	0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00 };

OXIDScanner::OXIDScanner()
{
	
}


OXIDScanner::~OXIDScanner()
{
	
}


void OXIDScanner::check(string& ipAddr){
	SOCKET clientSocket;
	TcpClient tcpClient = TcpClient();
	string receiveData;
	int packetSize;
	if (!tcpClient.initWinSock())
	{
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, ipAddr.c_str(), OXIDPORT))
			{
				if (!tcpClient.sendData(clientSocket, string((char*)IOXID_RPC_NEGOTIATE, sizeof(IOXID_RPC_NEGOTIATE) / sizeof(IOXID_RPC_NEGOTIATE[0]))))
				{
					if (!tcpClient.receiveData(clientSocket, receiveData, &packetSize))
					{
						receiveData.clear();
						if (!tcpClient.sendData(clientSocket, string((char*)IOXID_RPC_REQUEST, sizeof(IOXID_RPC_REQUEST) / sizeof(IOXID_RPC_REQUEST[0]))))
						{
							if (!tcpClient.receiveData(clientSocket, receiveData, &packetSize))
							{
								/**
								²Î¿¼ÁËhttps://github.com/uknowsec/OXID_Find/blob/master/OXID_Find/main.cpp
								*/
								char* p;
								p = (char*)receiveData.c_str();
								for (int i = 40; i < packetSize; i++) {
									if (p[i] != 0)
									{
										printf("%c", p[i]);
									}
									if (p[i + 1] == 9 && p[i + 2] == 0 && p[i + 3] == -1 && p[i + 4] == -1)
									{
										break;
									}
									if (p[i] == 0 && p[i + 1] == 0 && p[i + 2] == 0 && p[i + 3] == 7)
									{
										printf("\n\t[+] IP Address: ");
									}

								}
								closesocket(clientSocket);
							}
						}
					}
				}
			}
		}
	}
}