#include "m_multi_framework.h"

extern mutex g_vMutex;
extern mutex g_vServiceMutex;
/////////////////////
extern vector<NtlmInfo> g_vNtlmInfo; // save ntlm type2
extern vector<string> g_vServerLocalMemberAccount;
extern map<string, vector<string>> g_mServerLoggedAccount;
/////////////////////
extern neb::CJsonObject g_portServiceJson;
extern neb::CJsonObject g_portServiceBannerJson;

m_multi_framework::m_multi_framework(DWORD dwThreadNum){
	this->socketInit();
	this->createThreadPool(dwThreadNum);
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
		cout << "[+] Socket Init Successed" << endl;
	}
	else{
		cout << "[-] Socket Init Failed" << endl;
		exit(-1);
	}
}

void m_multi_framework::portBannerInit(){
	// port banner json init ....
	bool t1 = g_portServiceBannerJson.Parse(R"(
	[
		{
			"name": "docker",
			"probestring" : "GET /version HTTP/1.1\r\n\r\n",
			"pattern": ["KernelVersion"],
			"port": ["2375", "2379", "2380"]
		},
		{
			"name": "radmin",
			"probestring" : "\\x01\\x00\\x00\\x00\\x01\\x00\\x00\\x00\\x08\\x08",
			"pattern": ["^\\x01\\x00\\x00\\x00\\x25", "^\\x01\\x0b\\x00\\x00\\x00\\x08"],
			"port": ["4899", "9001"]
		},
		{
			"name": "openvpn",
			"probestring" : "\\x00\\x0e\\x38\\x37\\xa5\\x26\\x08\\xa2\\x1b\\xa0\\xb1\\x00\\x00\\x00\\x00\\x00",
			"pattern": ["^\\x00\\x1a\\x40","^\\x00\\x1e\\x40"],
			"port": ["1194", "443", "500"]
		},
		{
			"name": "pop3",
			"pattern": ["+OK "],
			"port": ["110", "995"]
		},
		{
			"name": "imap4",
			"pattern": ["* OK "],
			"port": ["143", "993"]
		},
		{
			"name": "telnet",
			"pattern": ["\\xff\\xfb\\x01\\xff\\xf0", "\\xff\\xfb\\x01\\xff\\xfb", "\\xff\\xfb\\x01\\xff\\xfc", "\\xff\\xfb\\x01\\xff\\xfd","\\xff\\xfb\\x01\\xff\\xfe","\\xff\\xfb\\x01\\x0a\\x0d","\\xff\\xfb\\x01\\x1b\\x5b"],
			"port": ["23"]
		},
		{
			"name": "smtp",
			"pattern": ["521 "],
			"port": ["25"]
		},
		{
			"name": "http",
			"probestring" : "GET / HTTP/1.1\r\n\r\n",
			"pattern": ["HTTP/1."],
			"port": ["80", "81", "82", "83", "84", "85", "888", "5985", "8000", "8080", "8161", "9200", "8888", "50000", "10000"]
		},
		{
			"name": "https",
			"probestring" : "GET / HTTP/1.1\r\n\r\n",
			"pattern": ["\\x16\\x03\\x00", "\\x16\\x03\\x01","\\x16\\x03\\x02","\\x16\\x03\\x03","\\x16\\x03\\x04"],
			"port": ["443", "993", "995", "1311", "1443", "3443", "4443", "5061", "5986", "7443", "8443", "8531", "9443", "10443", "14443", "44443", "60443"]
		},
		{
			"name": "http-proxy",
			"probestring" : "\\x47\\x45\\x54\\x20\\x68\\x74\\x74\\x70\\x3a\\x2f\\x2f\\x62\\x61\\x69\\x64\\x75\\x2e\\x63\\x6f\\x6d\\x2f\\x20\\x48\\x54\\x54\\x50\\x2f\\x31\\x2e\\x31\\x0d\\x0a\\x48\\x6f\\x73\\x74\\x3a\\x20\\x62\\x61\\x69\\x64\\x75\\x2e\\x63\\x6f\\x6d\\x0d\\x0a\\x0d\\x0a",
			"pattern": ["^HTTP/1"],
			"port": ["1080", "1090", "1095"]
		},
		{
			"name": "socks5",
			"probestring" : "\\x05\\x04\\x00\\x01\\x02\\x80\\x05\\x01\\x00\\x03\\x0a\\x67\\x6f\\x6f\\x67\\x6c\\x65\\x2e\\x63\\x6f\\x6d\\x00\\x50\\x47\\x45\\x54\\x20\\x2f\\x20\\x48\\x54\\x54\\x50\\x2f\\x31\\x2e\\x30\\x0d\\x0a\\x0d\\x0a",
			"pattern": ["^HTTP/1"],
			"port": ["1080", "1090", "1095"]
		},
		{
			"name": "socks4",
			"probestring" : "\\x04\\x01\\x00\\x16\\x7f\\x00\\x00\\x01\\x72\\x6f\\x6f\\x74\\x00",
			"pattern": ["^\\x00\\x5a","^\\x00\\x5b","^\\x00\\x5c","^\\x00\\x5d","^\\x00Access is denied\\x0a"],
			"port": ["1080", "1090", "1095"]
		},
		{
			"name": "smb",
			"probestring": "\\x00\\x00\\x00\\x85\\xff\\x53\\x4d\\x42\\x72\\x00\\x00\\x00\\x00\\x18\\x53\\xc0\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\xff\\xfe\\x00\\x00\\x40\\x00\\x00\\x62\\x00\\x02\\x50\\x43\\x20\\x4e\\x45\\x54\\x57\\x4f\\x52\\x4b\\x20\\x50\\x52\\x4f\\x47\\x52\\x41\\x4d\\x20\\x31\\x2e\\x30\\x00\\x02\\x4c\\x41\\x4e\\x4d\\x41\\x4e\\x31\\x2e\\x30\\x00\\x02\\x57\\x69\\x6e\\x64\\x6f\\x77\\x73\\x20\\x66\\x6f\\x72\\x20\\x57\\x6f\\x72\\x6b\\x67\\x72\\x6f\\x75\\x70\\x73\\x20\\x33\\x2e\\x31\\x61\\x00\\x02\\x4c\\x4d\\x31\\x2e\\x32\\x58\\x30\\x30\\x32\\x00\\x02\\x4c\\x41\\x4e\\x4d\\x41\\x4e\\x32\\x2e\\x31\\x00\\x02\\x4e\\x54\\x20\\x4c\\x4d\\x20\\x30\\x2e\\x31\\x32\\x00",
			"pattern": ["^\\x00\\x00\\x00.\\xff\\x53\\x4d\\x42\\x72.*"],
			"port": ["445"]
		},
		{
			"name": "vnc",
			"pattern": ["^RFB 00"],
			"port": ["5901"]
		}, 
		{
			"name": "rdp",
			"probestring": "\\x03\\x00\\x00\\x0b\\x06\\xe0\\x00\\x00\\x00\\x00\\x00",
			"pattern": ["^\\x03\\x00\\x00"],
			"port": ["3389", "3388", "33899"]
		},
		{
			"name": "mssql",
			"probestring": "\\x12\\x01\\x00\\x34\\x00\\x00\\x00\\x00\\x00\\x00\\x15\\x00\\x06\\x01\\x00\\x1b\\x00\\x01\\x02\\x00\\x1c\\x00\\x0c\\x03\\x00\\x28\\x00\\x04\\xff\\x08\\x00\\x01\\x55\\x00\\x00\\x00\\x4d\\x53\\x53\\x51\\x4c\\x53\\x65\\x72\\x76\\x65\\x72\\x00\\x48\\x0f\\x00\\x00",
			"pattern": ["MSSQLServer", "^\\x04\\x01\\x00\\x25\\x00\\x00\\x01\\x00\\x00\\x00\\x15"],
			"port": ["1433"]
		},
		{ 
			"name": "mysql",
			"pattern": ["^.*\\x00\\x00\\x00\\x0a", "\\x4a\\x00\\x00\\x00\\x0a\\x35", "mysql_native_password"],
			"port": ["3306"]
		},
		{
			"name": "postgresql",
			"pattern": ["invalid packet length", "^efatal"],
			"port": ["5432"]
		},
		{
			"name": "ssh",
			"pattern": ["^SSH-1.", "^SSH-2."],
			"port": ["22"]
		},
		{
			"name": "javarmi",
			"probestring": "\\x4a\\x52\\x4d\\x49\\x00\\x02\\x4b",
			"pattern": ["^\\x4e..[0-9a-f:.]+\\x00\\x00.."],
			"port": ["1099","9090"]
		},
		{
			"name": "activemq",
			"pattern": ["^ERROR\n(?:[^\n]+\n)?message:Unknown STOMP action:.+ org\.apache\.activemq\."],
			"port": ["61613", "6163"]
		},
		{
			"name": "zookeeper",
			"probestring": "stats\r\n",
			"pattern": ["^zookeeper version: "],
			"port": ["2181", "2182", "2183"]
		},
		{
			"name": "ftp",
			"pattern": ["^220.*\n331", "^220.*\n530", "^220.*FTP", "^220 .* Microsoft .* FTP", "^220 Inactivity timer", "^220 .* UserGate", "^220.*FileZilla Server"],
			"port": ["21"]
		},
		{
			"name": "ldap",
			"probestring": "\\x30\\x84\\x00\\x00\\x00\\x2d\\x02\\x01\\x07\\x63\\x84\\x00\\x00\\x00\\x24\\x04\\x00\\x0a\\x01\\x00\\x0a\\x01\\x00\\x02\\x01\\x00\\x02\\x01\\x64\\x01\\x01\\x00\\x87\\x0b\\x6f\\x62\\x6a\\x65\\x63\\x74\\x43\\x6c\\x61\\x73\\x73\\x30\\x84\\x00\\x00\\x00\\x00",
			"pattern": ["^\\x30\\x0c\\x02\\x01\\x01\\x61", "^\\x30\\x32\\x02\\x01", "^\\x30\\x33\\x02\\x01", "^\\x30\\x38\\x02\\x01", "^\\x30\\x84", "^\\x30\\x45"],
			"port": ["389"]
		},
		{
			"name": "mongodb",
			"probestring": "\\x41\\x00\\x00\\x00\\x3a\\x30\\x00\\x00\\xff\\xff\\xff\\xff\\xd4\\x07\\x00\\x00\\x00\\x00\\x00\\x00\\x74\\x65\\x73\\x74\\x2e\\x24\\x63\\x6d\\x64\\x00\\x00\\x00\\x00\\x00\\xff\\xff\\xff\\xff\\x1b\\x00\\x00\\x00\\x01\\x73\\x65\\x72\\x76\\x65\\x72\\x53\\x74\\x61\\x74\\x75\\x73\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\xf0\\x3f\\x00",
			"pattern": ["^.*version.....", "^\\xcb\\x00\\x00\\x00....:", "^.\\x00\\x00\\x00....:"],
			"port": ["27017", "9001", "49153"]
		},
		{
			"name": "ajp13",
			"probestring": "\\x12\\x34\\x00\\x01\\x0a",
			"pattern": ["^\\x41\\x42\\x00\\x01\\x09"],
			"port": ["8009","8008"]
		},
		{
			"name": "redis",
			"probestring" : "*1\r\n$4\r\ninfo\r\n",
			"pattern": ["^-ERR unknown command", "^-ERR wrong number of arguments", "^-DENIED Redis is running", "-ERR operation not permitted", "^\\$\\d+\\r\\n(?:#[^\\r\\n]*\\r\\n)*redis_version:([.\\d]+)\\r\\n"],
			"port": ["6379"]
		},
		{
			"name": "dubbo",
			"probestring" : "\r\n",
			"pattern": ["dubbo>"],
			"port": ["20880"]
		},
		{
			"name": "jdwp",
			"pattern": ["JDWP-Handshake"],
			"port": ["8000"]
		},
		{
			"name": "memcache",
			"probestring": "stats\r\n",
			"pattern": ["^STAT pid", "^Zookeeper version", "^ERROR\r\n"],
			"port": ["11211"]
		},
		{
			"name": "rsync",
			"pattern": ["^@RSYNCD:", "@RSYNCD:"],
			"port": ["873"]
		},
		{
			"name": "weblogic-t3",
			"probestring" : "t3 12.2.1\nAS:255\nHL:19\nMS:10000000\nPU:t3://us-l-breens:7001\n\n",
			"pattern": ["HELO:"],
			"port": ["7001"]
		},
		{
			"name": "couch-database",
			"probestring" : "\\x80\\x10\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x15\\xf0\\xd1\\x62\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00",
			"pattern": ["^\\x81\\x10"],
			"port": ["11210"]
		}
	]
	)");

	bool t2 = g_portServiceJson.Parse(R"([{"21": "FTP"},{"22": "SSH"},{"23": "Telnet"},{"25": "SMTP"},{"53": "DNS"},{"68": "DHCP"},{"69": "TFTP"},{"80": "HTTP"},{"443": "HTTPS"},{"8080": "HTTP"},{"995": "POP3"},{"135": "RPC"},{"139": "NetBIOS"},{"143": "IMAP"},{"443": "HTTPS"},{"161": "SNMP"},{"489": "LDAP"},{"445": "SMB"},{"465": "SMTPS"},{"512": "Linux_R_RPE"},{"513": "Linux_R_RLT"},{"514": "Linux_R_cmd"},{"873": "Rsync"},{"888": "BAOTA"},{"993": "IMAPS"},{"1080": "Proxy"},{"1099": "JavaRMI"},{"1352": "Lotus"},{"1433": "MSSQL"},{"1521": "Oracle"},{"1723": "PPTP"},{"2082": "CPanel"},{"2083": "CPanel"},{"2181": "Zookeeper"},{"2222": "DircetAdmin"},{"2375": "Docker"},{"2604": "Zebra"},{"3306": "MySQL"},{"3312": "Kangle"},{"3389": "RDP"},{"3690": "SVN"},{"4440": "Rundeck"},{"4848": "GlassFish"},{"5432": "PostgreSql"},{"5632": "PcAnywhere"},{"5900": "VNC"},{"5984": "CouchDB"},{"6082": "varnish"},{"6379": "Redis"},{"7001": "Weblogic"},{"8000": "Jdwp"},{"8009": "Ajp"},{"9001": "Weblogic"},{"7778": "Kloxo"},{"10050": "Zabbix"},{"8291": "RouterOS"},{"9200": "Elasticsearch"},{"11211": "Memcached"},{"27017": "MongoDB"},{"50070": "Hadoop"}])");
	if (t1 && t2){
		cout << "[+] PortBanner Init Successed" << endl;
	}
	else{
		cout << "[+] PortBanner Init Failed" << endl;
		exit(0);
	}
}

void m_multi_framework::createThreadPool(DWORD dwMaxThreadNum){
	DWORD dwhardwareThreadNum = thread::hardware_concurrency();
	if (dwMaxThreadNum < dwhardwareThreadNum){
		this->threadPool = new ThreadPool(dwMaxThreadNum, dwhardwareThreadNum);
	}else{
		this->threadPool = new ThreadPool(dwhardwareThreadNum, dwMaxThreadNum);
	}
	if (this->threadPool != NULL){
		cout << "[+] ThreadPool Init Successed" << endl;
	}
	else{
		cout << "[-] ThreadPool Init Failed" << endl;
		exit(-1);
	}

}

// 现在就是有多种情况，相关scanner的话就是 一个回调函数（参数为一个ip地址）
void m_multi_framework::addTask(callbackFunc func, map<string, string>* mArgs){
	this->threadPool->addTask(func, mArgs);
}

void m_multi_framework::addTask(callbackFunc2param func, map<string, string>* mArgs){
	this->threadPool->addTask(func, mArgs);
}

// 另一个就是弱口令的利用，那么这种的话就是 一个回调函数（参数为一个机器名称，自定义的一个弱口令）
void m_multi_framework::addTask(weakCallbackFunc func, map<string, string>* mArgs){
	this->threadPool->addTask(func, mArgs);
}

void m_multi_framework::startWork(){
	for (DWORD i = 0; i<this->threadPool->dwMaxThreadNum; i++){
		if (this->threadPool->consumeThread[i].joinable()){
			this->threadPool->consumeThread[i].join();}}
	this->threadPool->bShutDown = TRUE;
}

void m_multi_framework::getNtlmInfoResult(){
	for (UINT i = 0; i < g_vNtlmInfo.size(); i++)
	{
		printf("[+] %s\n", g_vNtlmInfo[i].ipAddr);
		wprintf(L"\tMACHINE_NAME: %s\n", g_vNtlmInfo[i].MACHINE_NAME);
		wprintf(L"\tNETBIOS_COMPUTER_NAME[0]: %s\n", g_vNtlmInfo[i].NETBIOS_ATTR[0].NETBIOS_COMPUTER_NAME);
		wprintf(L"\tNETBIOS_COMPUTER_NAME[1]: %s\n", g_vNtlmInfo[i].NETBIOS_ATTR[1].NETBIOS_COMPUTER_NAME);
		wprintf(L"\tDNS_COMPUTER_NAME[0]: %s\n", g_vNtlmInfo[i].DNS_ATTR[0].DNS_COMPUTER_NAME);
		wprintf(L"\tDNS_COMPUTER_NAME[1]: %s\n", g_vNtlmInfo[i].DNS_ATTR[1].DNS_COMPUTER_NAME);
	}
}


