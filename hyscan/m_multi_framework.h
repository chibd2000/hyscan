#ifndef THREADFRAMEWORK_H_H
#define THREADFRAMEWORK_H_H
#include "public.h"
#include "m_thread_pool.h"
#include "NtlmInfo.h"
#include "ScannerFactory.h"

class m_multi_framework
{
public:
	ThreadPool* threadPool;//			线程池
public:
	////////////////////////////////////
	m_multi_framework(DWORD dwThreadNum);
	~m_multi_framework();
	///////////////////////////////////
	void createThreadPool(DWORD dwMaxThreadNum); //线程池初始化
	void socketInit(); // socket ws库初始化
	static void portBannerInit(); // port banner初始化
	////////////////////////////////////
	void addTask(callbackFunc func, map<string, string>* mArgs); // 添加任务 1param
	void addTask(callbackFunc2param func, map<string, string>* vArgs); // 添加任务 2param
	void addTask(weakCallbackFunc func, map<string, string>* mArgs); // 添加任务 5param
	////////////////////////////////////
	void startWork(); // 开启任务，线程堵塞
	
	////////////////////////////////////
	void getNtlmInfoResult(); // ntlms type2 的结果获取
};
#endif
