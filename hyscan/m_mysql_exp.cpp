#include "m_mysql_exp.h"
#include "crypto_Sha1.h"
extern mutex g_vServiceMutex;
extern vector<ServiceVuln> g_vServiceVuln;
m_mysql_exp::m_mysql_exp(PortService portService)
{
	this->portService = portService;
}


m_mysql_exp::~m_mysql_exp()
{
}

void m_mysql_exp::checkServiceIsVuln(){
	if (!checkAliveReturn(this->portService.serviceIpAddr)){
		this->checkWeakpass();
	}
}

void m_mysql_exp::mysql_init(string& receiveData1){
	byte bSalt1[9] = { 0 };
	byte bSalt2[13] = { 0 };
	char szBuffer[100]{ 0 };
	char* p = NULL;
	memset(szBuffer, 0, 100);
	memcpy(szBuffer, receiveData1.c_str(), receiveData1.size());
	DWORD dwPacketSize = *(DWORD*)receiveData1.c_str();
	p = new char[dwPacketSize];
	char* pTemp = p;
	memcpy(pTemp, szBuffer + 4, dwPacketSize);
	pTemp++;
	int iVersionLength = strlen(pTemp + 1) + 2;
	pTemp += iVersionLength;
	int threadId = *(int*)pTemp;
	pTemp += 4;
	memcpy(bSalt1, pTemp, 9); // salt1
	pTemp += 9;
	pTemp += 18;
	memcpy(bSalt2, pTemp, 13); // salt2
	if (p != NULL){
		delete p;
		p = NULL;
	}
}

void m_mysql_exp::mysql_connect(){
	
}

string m_mysql_exp::hex2String(const char* data, size_t length){
	size_t i;
	char szBuffer[100] = { 0 };
	for (i = 0; i < length; i++) {
		sprintf(szBuffer, "%s%02x", szBuffer, (unsigned char)data[i]);
	}
	return szBuffer;
}

void m_mysql_exp::checkWeakpass(){
	string bugName = "weakpass";
	vector<string> vUsername{ "root", "mysql", "test" };
	vector<string> vPassword{ "root", "test", "mysql", "root123", "test123", "mysql123", 
		"root@123", "test@123", "123456", "mysql@123", "admin@123", "Aa@123456"};
	
	char native_password[] = { 0x6d, 0x79, 0x73, 0x71, 0x6c, 0x5f, 0x6e, 0x61, 0x74, 0x69, 
		0x76, 0x65, 0x5f, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64, 0x00 };
	
	TcpClient tcpClient;
	char szBuffer[MAX_PATH]{ 0 };
	char* p = NULL;
	int packetSize;
	string receiveData1;
	for (DWORD i = 0; i < vUsername.size(); i++){
		for (DWORD j = 0; j < vPassword.size(); j++){
			SOCKET clientSocket;
			if (!tcpClient.initWinSock())
			{
				if (!tcpClient.createSocket(clientSocket))
				{
					if (!tcpClient.connectSocket(clientSocket, this->portService.serviceIpAddr, this->portService.dwServicePort))
					{
						if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
						{
							byte bSalt1[9] = {};
							byte bSalt2[13] = {};
							char szBuffer[100]{ 0 };
							char* p = NULL;
							memset(szBuffer, 0, 100);
							memcpy(szBuffer, receiveData1.c_str(), packetSize);
							DWORD dwPacketSize = *(PDWORD)szBuffer;
							p = new char[dwPacketSize];
							RtlZeroMemory(p, dwPacketSize);
							memcpy(p, szBuffer + 4, dwPacketSize);
							char* pTemp = p;
							pTemp++;
							int iVersionLength = strlen(pTemp + 1) + 2;
							pTemp += iVersionLength;
							int threadId = *(int*)pTemp;
							pTemp += 4;
							memcpy(bSalt1, pTemp, 9); // salt1
							pTemp += 9;
							pTemp += 18;
							memcpy(bSalt2, pTemp, 13); // salt2
							pTemp = NULL;
							if (p != NULL){
								delete p;
								p = NULL;
							}
							// 开始通过salt1 salt2来进行加密
							memset(szBuffer, 0, 100);
							memcpy(szBuffer, bSalt1, 8);
							memcpy(szBuffer + 8, bSalt2, 12);
							// SHA1(password) XOR SHA1( "20-bytes random data from server" <concat> SHA1(SHA1(password)))
							string p1Bytes = SHA1::encode_bin(vPassword[j]);
							string randomBytes = SHA1::encode_bin(string(szBuffer) + SHA1::encode_bin(p1Bytes));
							string p3Bytes;
							p3Bytes.resize(20);
							for (DWORD p = 0; p<randomBytes.size(); p++){ p3Bytes[p] = p1Bytes[p] ^ randomBytes[p]; }

							// 开始拼接认证包
							char* p1 = new char[100];
							RtlZeroMemory(p1, 100);
							pTemp = p1;
							pTemp += 3;
							*pTemp = 0x01;
							pTemp++;
							*(PWORD)pTemp = 0xa685;
							pTemp += 2;
							*pTemp = 0xff;
							pTemp++;
							*pTemp = 0x01;
							pTemp++;
							*(PDWORD)pTemp = 0x1000000;
							pTemp += 4;
							*pTemp = 0x21;
							pTemp += 2;
							pTemp += 22;
							strcpy(pTemp, vUsername[i].c_str());
							pTemp += vUsername[i].size();
							*(PWORD)pTemp = 0x1400;
							pTemp += 2;
							memcpy(pTemp, p3Bytes.data(), p3Bytes.size());
							pTemp += p3Bytes.size();
							memcpy(pTemp, native_password, 22);
							pTemp += 22;
							DWORD dwRangeSize = (DWORD)pTemp - (DWORD)p1 - 4;
							*(PWORD)p1 = dwRangeSize;
							string finaDataString = string(p1, dwRangeSize+4);
							pTemp = NULL;
							if (p1 != NULL){
								delete p1;
								p1 = NULL;
							}
							receiveData1.clear();
							Sleep(50);
							if (!tcpClient.sendData(clientSocket, finaDataString))
							{
								if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
								{
									pTemp = (char*)receiveData1.c_str();
									pTemp += 5;
									if (*pTemp == 0){
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
							closesocket(clientSocket);
						}
					}
				}
			}
		}
	}
	return;
}
