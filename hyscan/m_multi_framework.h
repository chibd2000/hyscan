#ifndef THREADFRAMEWORK_H_H
#define THREADFRAMEWORK_H_H
#include "public.h"
#include "m_thread_pool.h"
#include "m_wnet_api.h"
#include "NtlmInfo.h"
#include "ScannerFactory.h"
class m_multi_framework
{
public:
	ThreadPool* threadPool;//			Ïß³Ì³Ø
public:
	////////////////////////////////////
	m_multi_framework(DWORD dwThreadNum);
	~m_multi_framework();
	void socketInit();
	////////////////////////////////////
	void createThreadPool(DWORD dwMaxThreadNum);
	void addTask(callbackFunc func, map<string, string>* mArgs);
	void addTask(weakCallbackFunc func, map<string, string>* mArgs);
	////////////////////////////////////
	void startWork();
	
	////////////////////////////////////
	void getNtlmInfoResult();
};
#endif
