#include "m_wmi_api.h"
#include "Tools.h"

WMIAPI::WMIAPI()
{
	pWbemSvc = NULL;
	pWbemLoc = NULL;
	pWbemClsObj = NULL;
	pEnumClsObj = NULL;
}


WMIAPI::~WMIAPI()
{
	
}

// https://docs.microsoft.com/en-us/windows/win32/api/wbemcli/nn-wbemcli-iwbemservices

HRESULT WMIAPI::init(){
	HRESULT hr;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
	{
		hr = CoInitializeSecurity(NULL,
			-1,
			NULL,
			NULL,
			RPC_C_AUTHN_LEVEL_PKT,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE,
			NULL);
		if (SUCCEEDED(hr)){
			//二、创建一个WMI命名空间连接，创建一个CLSID_WbemLocator对象  
			hr = CoCreateInstance(CLSID_WbemLocator,
				0,
				CLSCTX_INPROC_SERVER,
				IID_IWbemLocator,
				(LPVOID*)&pWbemLoc);
			return hr;
		}
	}
	return hr;
}

void WMIAPI::release(){
	if (pWbemSvc != NULL)
	{
		pWbemSvc->Release();
		pWbemSvc = NULL;
	}
	if (pWbemLoc != NULL)
	{
		pWbemLoc->Release();
		pWbemLoc = NULL;

	}
	if (pEnumClsObj != NULL)
	{
		pEnumClsObj->Release();
		pEnumClsObj = NULL;
	}
	CoUninitialize();
}

void WMIAPI::connect(string& machineName){
	HRESULT hr;
	wstring connectAddr;
	hr = this->init();
	if (SUCCEEDED(hr) || RPC_E_CHANGED_MODE == hr)
	{
		//设置进程的安全级别，（调用COM组件时在初始化COM之后要调用CoInitializeSecurity设置进程安全级别，否则会被系统识别为病毒）  
		connectAddr.append(L"\\\\");
		connectAddr.append(string2Wstring(machineName));
		connectAddr.append(L"\\root\\CIMV2");
		hr = pWbemLoc->ConnectServer(
			(BSTR)connectAddr.c_str(),  // Namespace
			NULL,          // Userid
			NULL,           // PW
			NULL,           // Locale
			0,              // flags
			NULL,           // Authority
			NULL,           // Context
			&pWbemSvc
			);
		if (SUCCEEDED(hr)){
			cout << "\t[+] " << machineName << " Connection success" << endl;
		}
	}
}

void WMIAPI::exec(string& command){
	
}
