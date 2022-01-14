#ifndef PORTSCANNER_H_H
#define PORTSCANNER_H_H
#include "public.h"
enum ServicePort{
	Dubbo = 0x00,
	FTP = 0x05,
	JDWP = 1,
	MEMCACHE = 0x43,
	MONGODB = 0x52E,
	MSSQL = 1,
	MYSQL = 2,
	T3 = 2,
	RSYNC=1,
	REDIS=3,
	POSTGREAL=2,
	ZOOKEEPER = 3,
	POSTGRESQL = 3
};

class ServiceVuln{
public:
	BOOL vulnerable;
	DWORD vulnPort;
	string serviceName;
	string username;
	string password;
};

class ServiceScanner{
public:
	DWORD checkPORT;
	virtual void scan();
};

class DubboServiceScanner:public ServiceScanner{
public:
	DubboServiceScanner(){
		this->checkPORT = 28880;
	}
	~DubboServiceScanner(){

	}
};

class FtpServiceScanner:public ServiceScanner{
public:
	FtpServiceScanner(){
		this->checkPORT = 21;
	}
	~FtpServiceScanner(){

	}
};

class JdwpServiceScanner :public ServiceScanner{
	JdwpServiceScanner(){
		this->checkPORT = 21;
	}
	~JdwpServiceScanner(){

	}
};

class MemcacheServiceScanner :public ServiceScanner{
	MemcacheServiceScanner(){
		this->checkPORT = 21;
	}
	~MemcacheServiceScanner(){

	}
};

class MongodbServiceScanner :public ServiceScanner{
	MongodbServiceScanner(){
		this->checkPORT = 21;
	}
	~MongodbServiceScanner(){

	}
};

class MssqlServiceScanner :public ServiceScanner{
	MssqlServiceScanner(){
		this->checkPORT = 21;
	}	
	~MssqlServiceScanner(){

	}
};

class MysqlServiceScanner :public ServiceScanner{
	MysqlServiceScanner(){
		this->checkPORT = 21;
	}
	~MysqlServiceScanner(){
		
	}
};

class RedisServiceScanner :public ServiceScanner{
	RedisServiceScanner(){
		this->checkPORT = 21;
	}
	~RedisServiceScanner(){

	}
};

class PostgresqlServiceScanner :public ServiceScanner{
	PostgresqlServiceScanner(){
		this->checkPORT = 21;
	}
	~PostgresqlServiceScanner(){

	}
};

class RMIServiceScanner :public ServiceScanner{
	RMIServiceScanner(){
		this->checkPORT = 21;
	}
	~RMIServiceScanner(){

	}
};

class RsyncServiceScanner :public ServiceScanner{
	RsyncServiceScanner(){
		this->checkPORT = 21;
	}
	~RsyncServiceScanner(){

	}
};

class ZookeeperServiceScanner :public ServiceScanner{
	ZookeeperServiceScanner(){
		this->checkPORT = 21;
	}
	~ZookeeperServiceScanner(){

	}
};

class T3ServiceScanner :public ServiceScanner{
	T3ServiceScanner(){
		this->checkPORT = 21;
	}
	~T3ServiceScanner(){

	}
};

class PortScanner
{
public:
	PortScanner();
	void checkPortStatus(); // 检查是否开放端口
	void checkService(); // 检查是什么服务
	ServiceScanner* serviceScanner;
	~PortScanner();
};

#endif
