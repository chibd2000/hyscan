#include "c_service.h"


PortService::PortService()
{
	
}


PortService::PortService(string serviceNameString, string serviceIpAddr, DWORD dwServicePort, DWORD dwPortStatus, DWORD dwServiceStatus){
	this->serviceNameString = serviceNameString;
	this->serviceIpAddr = serviceIpAddr;
	this->dwServicePort = dwServicePort;
	this->dwPortStatus = dwPortStatus;
	this->dwServiceStatus = dwServiceStatus;
}

PortService::~PortService()
{
	
}

// ================================

ServiceVuln::ServiceVuln()
{

}

ServiceVuln::ServiceVuln(string serviceName, BOOL vulnerable, BOOL bIsBug, DWORD vulnPort, string bugInformation){
	this->vulnerable = vulnerable;
	this->bIsBug = bIsBug;
	this->vulnPort = vulnPort;
	this->serviceName = serviceName;
	this->bugInformation = bugInformation;
}

ServiceVuln::~ServiceVuln()
{

}

