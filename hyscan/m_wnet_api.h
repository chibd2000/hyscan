#ifndef WNETAPI_H_H
#define WNETAPI_H_H
#include "m_base_api.h"
#include <lm.h>
#pragma comment(lib, "Netapi32.lib")
class WNET_API: public MYAPI
{
private:
	string username;
	string password;
	string serverName;
public:
	WNET_API(string& serverName);
	void openConnect();
	void closeConnection();
	void enumLoggedUser(wstring serverName);
	~WNET_API();
};
#endif
