#ifndef WMIAPI_H_H
#define WMIAPI_H_H
#include "m_w_base_api.h"
#include <WbemIdl.h>
#pragma comment(lib,"WbemUuid.lib")
class WMIAPI : public m_w_base_api
{
private:
	IEnumWbemClassObject* pEnumClsObj; // 用于 IWbemClassObject 类型的对象的枚举器。 它类似于标准 COM 枚举器，如 IEnumVariant。
	IWbemClassObject* pWbemClsObj; // 	包含并操作类定义和类对象实例。 开发人员不需要实现此接口;WMI 提供其实现。
	IWbemServices* pWbemSvc; // 客户端和提供程序用于访问 WMI 服务。 接口仅由 WMI 实现，是主 WMI 接口
	IWbemLocator* pWbemLoc; // 获取指向特定主计算机上 WMI 的 IWbemServices 接口的初始命名空间指针
public:
	WMIAPI();
	HRESULT init();
	HRESULT connect(string& ipAddr, string& username, string& password);
	HRESULT connect(string& ipAddr);
	void exec(string& command);
	void release();
	~WMIAPI();
};

#endif