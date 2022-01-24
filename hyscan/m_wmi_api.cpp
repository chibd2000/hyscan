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
		// 设置进程的安全级别，（调用COM组件时在初始化COM之后要调用CoInitializeSecurity设置进程安全级别，否则会被系统识别为病毒）  
		hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
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

HRESULT WMIAPI::connect(string& ipAddr){
	HRESULT hr;
	do{
		hr = this->init();
		if (SUCCEEDED(hr)){
			wstring connectAddr;
			connectAddr.append(L"\\\\");
			connectAddr.append(string2Wstring(ipAddr).data());
			connectAddr.append(L"\\root\\cimv2");
			hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pWbemLoc);
			if (SUCCEEDED(hr)){
				hr = pWbemLoc->ConnectServer((WCHAR*)connectAddr.c_str(), NULL, NULL, 0, NULL, 0, 0, &pWbemSvc);
				if (SUCCEEDED(hr)){
					this->release();
					return 1;
				}
				else{
					this->release();
					return 0;
				}
			}
			else{
				this->release();
				return 0;
			}
		}
		else{
			this->release();
			return 0;
		}
	} while (false);

	return 0;
}

void WMIAPI::exec(string& command){
	
}
