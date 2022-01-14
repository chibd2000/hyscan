#include "m_multi_framework.h"

extern vector<NtlmInfo> vNtlmInfo; // save ntlm type2
extern vector<string> vServerLocalMemberAccount;
extern map<string, vector<string>> mServerLoggedAccount;

m_multi_framework::m_multi_framework(DWORD dwThreadNum){
	this->createThreadPool(dwThreadNum);
	this->socketInit();
}

m_multi_framework::~m_multi_framework()
{
	/*
	if (this->threadPool != NULL){
		delete this->threadPool;
		this->threadPool = NULL;
	}*/ 
#ifdef DEBUG
	cout << "[+] TaskSize		----> " << this->threadPool->getQueueTaskSize() << endl;
	cout << "[+] workThreadNum	----> " << this->threadPool->workThreadNum << endl;
	cout << "[+] aliveThreadNum	----> " << this->threadPool->aliveThreadNum << endl;
#else
#endif
}

void m_multi_framework::socketInit(){
	WSADATA wsaData;
	if (!WSAStartup(MAKEWORD(2, 2), &wsaData)){
		cout << "[+] socketInit Successed" << endl;
	}
	else{
		cout << "[-] socketInit Failed" << endl;
		exit(-1);
	}
}

void m_multi_framework::createThreadPool(DWORD dwMaxThreadNum){
	DWORD dwhardwareThreadNum = thread::hardware_concurrency();
	if (dwMaxThreadNum < dwhardwareThreadNum){
		this->threadPool = new ThreadPool(dwMaxThreadNum, dwhardwareThreadNum);
	}else{
		this->threadPool = new ThreadPool(dwhardwareThreadNum, dwMaxThreadNum);
	}
}

// 现在就是有多种情况，相关scanner的话就是 一个回调函数（参数为一个ip地址）
void m_multi_framework::addTask(callbackFunc func, map<string, string>* vArgs){
	this->threadPool->addTask(func, vArgs);
}

// 另一个就是弱口令的利用，那么这种的话就是 一个回调函数（参数为一个机器名称，自定义的一个弱口令）
void m_multi_framework::addTask(weakCallbackFunc func, map<string, string>* vArgs){
	this->threadPool->addTask(func, vArgs);
}

void m_multi_framework::startWork(){
	for (DWORD i = 0; i<this->threadPool->dwMaxThreadNum; i++){
		if (this->threadPool->consumeThread[i].joinable()){
			this->threadPool->consumeThread[i].join();}}
	this->threadPool->bShutDown = TRUE;
}

void m_multi_framework::getNtlmInfoResult(){
	cout << vNtlmInfo.size() << endl;
	for (UINT i = 0; i < vNtlmInfo.size(); i++)
	{
		printf("[+] %s\n", vNtlmInfo[i].ipAddr);
		wprintf(L"\tMACHINE_NAME: %s\n", vNtlmInfo[i].MACHINE_NAME);
		wprintf(L"\tNETBIOS_COMPUTER_NAME[0]: %s\n", vNtlmInfo[i].NETBIOS_ATTR[0].NETBIOS_COMPUTER_NAME);
		wprintf(L"\tNETBIOS_COMPUTER_NAME[1]: %s\n", vNtlmInfo[i].NETBIOS_ATTR[1].NETBIOS_COMPUTER_NAME);
		wprintf(L"\tDNS_COMPUTER_NAME[0]: %s\n", vNtlmInfo[i].DNS_ATTR[0].DNS_COMPUTER_NAME);
		wprintf(L"\tDNS_COMPUTER_NAME[1]: %s\n", vNtlmInfo[i].DNS_ATTR[1].DNS_COMPUTER_NAME);
	}
}


