#ifndef UserInfo_H_H
#define UserInfo_H_H
#include "public.h"

/* write in 2021.12.14 15.51 for userinfo @zpchcbd*/
class c_userInfo
{
public:
	string ip;
	string domainName;
	string machineName;
	string username;
	string password;
public:
	c_userInfo();
	c_userInfo(string ip, string domainName, string machineName, string username, string password);
	~c_userInfo();
};

#endif
