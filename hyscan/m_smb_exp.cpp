#include "m_smb_exp.h"

extern mutex g_vServiceMutex;
extern vector<ServiceVuln> g_vServiceVuln;

m_smb_exp::m_smb_exp(PortService portService)
{
	this->portService = portService;
}


m_smb_exp::~m_smb_exp()
{
}

void m_smb_exp::checkServiceIsVuln(){
	if (!s_net_scanner::checkAliveReturn(this->portService.serviceIpAddr)){
		this->checkEternalBlue();
		this->checkSMBGhost();
	}
}

void m_smb_exp::checkEternalBlue(){
	string bugName = "MS17010";
	SOCKET clientSocket;
	TcpClient tcpClient;

	string negotiateProtocolRequest = getHexString(R"(\x00\x00\x00\x85\xff\x53\x4d\x42\x72\x00\x00\x00\x00\x18\x53\xc0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xfe\x00\x00\x40\x00\x00\x62\x00\x02\x50\x43\x20\x4e\x45\x54\x57\x4f\x52\x4b\x20\x50\x52\x4f\x47\x52\x41\x4d\x20\x31\x2e\x30\x00\x02\x4c\x41\x4e\x4d\x41\x4e\x31\x2e\x30\x00\x02\x57\x69\x6e\x64\x6f\x77\x73\x20\x66\x6f\x72\x20\x57\x6f\x72\x6b\x67\x72\x6f\x75\x70\x73\x20\x33\x2e\x31\x61\x00\x02\x4c\x4d\x31\x2e\x32\x58\x30\x30\x32\x00\x02\x4c\x41\x4e\x4d\x41\x4e\x32\x2e\x31\x00\x02\x4e\x54\x20\x4c\x4d\x20\x30\x2e\x31\x32\x00)");
	/*
	smb_header = [
	'\xFF\x53\x4D\x42',  # 'server_component': .SMB
	'\x72',              # 'smb_command': Negotiate Protocol
	'\x00\x00\x00\x00',  # 'nt_status'
	'\x18',              # 'flags'
	'\x01\x28',          # 'flags2'
	'\x00\x00',          # 'process_id_high'
	'\x00\x00\x00\x00\x00\x00\x00\x00',  # 'signature'
	'\x00\x00',          # 'reserved'
	'\x00\x00',          # 'tree_id'
	'\x2F\x4B',          # 'process_id'
	'\x00\x00',          # 'user_id'
	'\xC5\x5E'           # 'multiplex_id'
	]
	*/
	
	string sessionSetupRequest = getHexString("\\x00\\x00\\x00\\x88\\xff\\x53\\x4d\\x42\\x73\\x00\\x00\\x00\\x00\\x18\\x07\\xc0\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\xff\\xfe\\x00\\x00\\x40\\x00\\x0d\\xff\\x00\\x88\\x00\\x04\\x11\\x0a\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x01\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\xd4\\x00\\x00\\x00\\x4b\\x00\\x00\\x00\\x00\\x00\\x00\\x57\\x00\\x69\\x00\\x6e\\x00\\x64\\x00\\x6f\\x00\\x77\\x00\\x73\\x00\\x20\\x00\\x32\\x00\\x30\\x00\\x30\\x00\\x30\\x00\\x20\\x00\\x32\\x00\\x31\\x00\\x39\\x00\\x35\\x00\\x00\\x00\\x57\\x00\\x69\\x00\\x6e\\x00\\x64\\x00\\x6f\\x00\\x77\\x00\\x73\\x00\\x20\\x00\\x32\\x00\\x30\\x00\\x30\\x00\\x30\\x00\\x20\\x00\\x35\\x00\\x2e\\x00\\x30\\x00\\x00\\x00");
	
	/*
	session_setup_andx_request = [
	'\x0D',              # Word Count
	'\xFF',              # AndXCommand: No further command
	'\x00',              # Reserved
	'\x00\x00',          # AndXOffset
	'\xDF\xFF',          # Max Buffer
	'\x02\x00',          # Max Mpx Count
	'\x01\x00',          # VC Number
	'\x00\x00\x00\x00',  # Session Key
	'\x00\x00',          # ANSI Password Length
	'\x00\x00',          # Unicode Password Length
	'\x00\x00\x00\x00',  # Reserved
	'\x40\x00\x00\x00',  # Capabilities
	'\x26\x00',          # Byte Count
	'\x00',              # Account
	'\x2e\x00',          # Primary Domain
	'\x57\x69\x6e\x64\x6f\x77\x73\x20\x32\x30\x30\x30\x20\x32\x31\x39\x35\x00',    # Native OS: Windows 2000 2195
	'\x57\x69\x6e\x64\x6f\x77\x73\x20\x32\x30\x30\x30\x20\x35\x2e\x30\x00',        # Native OS: Windows 2000 5.0
	]
	*/
	string treeConnectRequest = getHexString("\\x00\\x00\\x00\\x60\\xff\\x53\\x4d\\x42\\x75\\x00\\x00\\x00\\x00\\x18\\x07\\xc0\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\xff\\xfe\\x00\\x08\\x40\\x00\\x04\\xff\\x00\\x60\\x00\\x08\\x00\\x01\\x00\\x35\\x00\\x00\\x5c\\x00\\x5c\\x00\\x31\\x00\\x39\\x00\\x32\\x00\\x2e\\x00\\x31\\x00\\x36\\x00\\x38\\x00\\x2e\\x00\\x31\\x00\\x37\\x00\\x35\\x00\\x2e\\x00\\x31\\x00\\x32\\x00\\x38\\x00\\x5c\\x00\\x49\\x00\\x50\\x00\\x43\\x00\\x24\\x00\\x00\\x00\\x3f\\x3f\\x3f\\x3f\\x3f\\x00");
	string transNamedPipeRequest = getHexString("\\x00\\x00\\x00\\x4a\\xff\\x53\\x4d\\x42\\x25\\x00\\x00\\x00\\x00\\x18\\x01\\x28\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x08\\x8e\\xa3\\x01\\x08\\x52\\x98\\x10\\x00\\x00\\x00\\x00\\xff\\xff\\xff\\xff\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x4a\\x00\\x00\\x00\\x4a\\x00\\x02\\x00\\x23\\x00\\x00\\x00\\x07\\x00\\x5c\\x50\\x49\\x50\\x45\\x5c\\x00");
	string trans2SessionSetupRequest = getHexString("\\x00\\x00\\x00\\x4e\\xff\\x53\\x4d\\x42\\x32\\x00\\x00\\x00\\x00\\x18\\x07\\xc0\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x08\\xff\\xfe\\x00\\x08\\x41\\x00\\x0f\\x0c\\x00\\x00\\x00\\x01\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\xa6\\xd9\\xa4\\x00\\x00\\x00\\x0c\\x00\\x42\\x00\\x00\\x00\\x4e\\x00\\x01\\x00\\x0e\\x00\\x0d\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00");
	
	char szBuffer[100] = { 0 };

	int packetSize;
	string receiveData1;

	if (!tcpClient.initWinSock())
	{
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, this->portService.serviceIpAddr, this->portService.dwServicePort))
			{
				if (!tcpClient.sendData(clientSocket, negotiateProtocolRequest))
				{
					Sleep(50);
					if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
					{
						receiveData1.clear();
						if (!tcpClient.sendData(clientSocket, sessionSetupRequest))
						{
							Sleep(50);
							if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
							{
								string userIdString = receiveData1.substr(32,2);
								treeConnectRequest[32] = userIdString[0];
								treeConnectRequest[33] = userIdString[1];
								receiveData1.clear();
								if (!tcpClient.sendData(clientSocket, treeConnectRequest))
								{
									Sleep(50);
									if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
									{
										string treeIDString = receiveData1.substr(28,2);
										transNamedPipeRequest[28] = treeIDString[0];
										transNamedPipeRequest[29] = treeIDString[1];
										transNamedPipeRequest[32] = userIdString[0];
										transNamedPipeRequest[33] = userIdString[1];
										receiveData1.clear();
										if (!tcpClient.sendData(clientSocket, transNamedPipeRequest))
										{
											Sleep(50);
											// reply[9] == 0x05 && reply[10] == 0x02 && reply[11] == 0x00 && reply[12] == 0xc0
											if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
											{
												if ((unsigned char)receiveData1[9] == 0x05 && (unsigned char)receiveData1[10] == 0x02 && (unsigned char)receiveData1[11] == 0x00 && (unsigned char)receiveData1[12] == 0xc0){
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
						}
					}
				}
			}
		}
	}
	closesocket(clientSocket);
}

void m_smb_exp::checkSMBGhost(){
	string bugName = "SMBGhost";
	SOCKET clientSocket;
	TcpClient tcpClient;
	
	string payload = "\\x00\\x00\\x00\\xc0\\xfe\\x53\\x4d\\x42\\x40\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x1f\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x24\\x00\\x08\\x00\\x01\\x00\\x00\\x00\\x7f\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x78\\x00\\x00\\x00\\x02\\x00\\x00\\x00\\x02\\x02\\x10\\x02\\x22\\x02\\x24\\x02\\x00\\x03\\x02\\x03\\x10\\x03\\x11\\x03\\x00\\x00\\x00\\x00\\x01\\x00\\x26\\x00\\x00\\x00\\x00\\x00\\x01\\x00\\x20\\x00\\x01\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x03\\x00\\x0a\\x00\\x00\\x00\\x00\\x00\\x01\\x00\\x00\\x00\\x01\\x00\\x00\\x00\\x01\\x00\\x00\\x00\\x00\\x00\\x00\\x00";
	char szBuffer[100] = { 0 };

	int packetSize;
	string receiveData1;

	if (!tcpClient.initWinSock())
	{
		if (!tcpClient.createSocket(clientSocket))
		{
			if (!tcpClient.connectSocket(clientSocket, this->portService.serviceIpAddr, this->portService.dwServicePort))
			{	
				if (!tcpClient.sendData(clientSocket, getHexString(payload)))
				{
					if (!tcpClient.receiveData(clientSocket, receiveData1, &packetSize))
					{
						DWORD dwPacketSize = ntohl(*(DWORD*)receiveData1.c_str());
						if (dwPacketSize > 0){
							string realPacketContent = receiveData1.substr(4, dwPacketSize);
							if (realPacketContent.substr(68, 2) == "\x11\x03" || realPacketContent.substr(68, 2) == "\x02\x00"){
								memset(szBuffer, 0, 100);
								sprintf(szBuffer, "%s %s %s", this->portService.serviceIpAddr.data(), this->portService.serviceNameString.data(), bugName.data());
								g_vServiceMutex.lock();
								g_vServiceVuln.push_back(ServiceVuln("SMB", true, true, this->portService.dwServicePort, szBuffer));
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
	closesocket(clientSocket);
}
