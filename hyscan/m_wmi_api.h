#ifndef WMIAPI_H_H
#define WMIAPI_H_H
#include "m_w_base_api.h"
#include <WbemIdl.h>
#pragma comment(lib,"WbemUuid.lib")
class WMIAPI : public m_w_base_api
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