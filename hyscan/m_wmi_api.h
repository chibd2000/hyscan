#ifndef WMIAPI_H_H
#define WMIAPI_H_H
#include "m_base_api.h"
#include <WbemIdl.h>
#pragma comment(lib,"WbemUuid.lib")
class WMIAPI: public MYAPI
{
private:
	IEnumWbemClassObject* pEnumClsObj;
	IWbemClassObject* pWbemClsObj;
	IWbemServices* pWbemSvc;
	IWbemLocator* pWbemLoc;
public:
	WMIAPI();
	HRESULT init();
	void connect(string& machineName, string& username, string& password);
	void connect(string& machineName);
	void exec(string& command);
	void release();
	~WMIAPI();
};

#endif