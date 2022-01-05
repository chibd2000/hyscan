#ifndef UserInfo_H_H
#define UserInfo_H_H
#include "public.h"

/* write in 2021.12.14 15.51 for userinfo @zpchcbd*/
class UserInfo
{
public:
	string ip;
	string domain;
	string machineName;
	string username;
	string password;
public:
	UserInfo();
	UserInfo(string ip, string domain, string machineName, string username, string password);
	~UserInfo();
};

#endif
