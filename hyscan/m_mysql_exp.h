#ifndef MYSQLSERVICEEXP_H_H
#define MYSQLSERVICEEXP_H_H
#include "m_base_exp.h"

class m_mysql_exp: public m_base_exp
{
public:
	m_mysql_exp(PortService portService);
	~m_mysql_exp();
	void checkServiceIsVuln();
	void checkWeakpass();
	void mysql_init(string& receiveData1);
	void mysql_connect();
	string hex2String(const char* data, size_t length);
};


struct MYSQL_GREET{
	byte protocol_version; // 0x0a 第一个字节表示协议版本号
	char server_version[10]; // 服务器版本号，字符串遇到Null结束
	DWORD connection_id; // connection id 服务器线程id
	long long auth_plugin_data_part_1; // 第一部分8个字节的挑战随机数，后面还有第二部分
	byte filler_1; //  0x00 填充位0x00
	byte capability_flag_1[2]; // 服务器权能标志（低位2个字节）
	unsigned long long character_set; // character_set (1) -- default server character-set, only the lower 8-bits Protocol::CharacterSet (optional)
	byte status_flags[2];
	byte capability_flags_2[2];
	byte auth_plugin_data_len[2]; // 挑战随机数的长度
	char reserved[10];
	byte auth_plugin_data_part_2[12]; // 挑战随机数的第二部分，通常是12字节 结束标志00
	string auth_plugin_name; // 认证插件的名称，null结尾
};

struct MYSQL_AUTH{	
	byte capability_flags[4]; //  capability flags of the client as defined in Protocol::CapabilityFlags
	DWORD max_packet_size; // max size of a command packet that the client wants to send to the server
	byte character_set; // connection's default character set as defined in Protocol::CharacterSet.
	char blank[22];
	char username[10];
	byte o_14 = 0x14;
	char database[10];
	char password[20];
	char auth_plugin_name[22];
};
#endif
