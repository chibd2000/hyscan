#ifndef PORTSCANNER_H_H
#define PORTSCANNER_H_H
#include "public.h"
#include "m_socket_api.h"
/////////////////////////
#include "m_dubbo_exp.h"
#include "m_ftp_exp.h"
#include "m_javarmi_exp.h"
#include "m_ldap_exp.h"
#include "m_mongodb_exp.h"
#include "m_mysql_exp.h"
#include "m_redis_exp.h"
#include "m_rsync_exp.h"
#include "m_smb_exp.h"
#include "m_zookeeper_exp.h"
#include "m_memcache_exp.h"
#include "m_mssql_exp.h"
/////////////////////////
#include "Tools.h"

class PortScanner
{
public:
	PortScanner();
	static void serviceCheck(string& ipAddr, string& port);
	~PortScanner();
};

enum HENGGE_PORT_STATUS{
	PORT_OPEN = 1,
	PORT_CLOSE = -1,
	PORT_UNKNOWN = 0
};

enum HENGGE_SERVICE_STATUS{
	SERVICE_KNOW = 1,
	SERVICE_NO_KNOWN = -1,
	SERVICE_UNKNOWN = 0
};

// PortScanner
// ServiceScanner
// banner
// 

class ServiceScanner{
public:
	void serviceCheck(string ipAddr, DWORD dwPort, string scanType);
	void tcpServiceCheck(string ipAddr, DWORD dwPort);
	void udpServiceCheck(string ipAddr, DWORD dwPort);
	void serviceExploit(PortService portService);
	string getServicebyPortList(DWORD dwPort);
};

#endif
