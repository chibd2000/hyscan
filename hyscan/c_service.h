#ifndef SERVICE_H_H
#define SERVICE_H_H
#include "public.h"

class PortService{
public:
	PortService();
	PortService(string serviceNameString, string serviceIpAddr, DWORD dwServicePort, DWORD dwPortStatus, DWORD dwServiceStatus);
	~PortService();
public:
	string serviceNameString;
	string serviceIpAddr;
	DWORD dwServicePort;
	DWORD dwPortStatus;
	DWORD dwServiceStatus;
};

class ServiceVuln{

public:
	ServiceVuln();
	ServiceVuln(string serviceName, BOOL vulnerable, BOOL bIsBug, DWORD vulnPort, string bugInformation);
	~ServiceVuln();
public:
	BOOL vulnerable; //是否可以进行漏洞利用
	BOOL bIsBug; // 漏洞是否存在
	DWORD vulnPort; // 漏洞端口
	string serviceName; // 服务名称
	string bugInformation; // 相关描述信息
};

#endif

