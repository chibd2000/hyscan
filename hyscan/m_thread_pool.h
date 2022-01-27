#ifndef THREADPOOL_H_H
#define THREADPOOL_H_H
#define THREAD_CLOSE_NUM 5
#include "public.h"
#include <condition_variable>
#include "c_service.h"
using callbackFunc = void(*)(string& arg); // scanner
using callbackFunc2param = void(*)(string& arg, string& arg2); // scanner
using callbackFunc3param = void(*)(string& arg, string& arg2, string& arg3); // scanner
using callbackFunc4param = void(*)(string& arg, string& arg2, string& arg3, string& arg4); // scanner
using weakCallbackFunc = void(*)(string& arg1, string& arg2, string& arg3, string& arg4, string& arg5);

class Task{
public:
	callbackFunc func;
	weakCallbackFunc weakFunc;
	callbackFunc2param param2Func;
	callbackFunc3param param3Func;
	map<string, string>* mArgs;
public:
	Task(){
		this->func = nullptr;
		this->mArgs = nullptr;
	}

	// 到时候可以改，应该要改为模板函数来写，这样就不需要根据参数不同来添加函数了
	Task(callbackFunc func, map<string, string>* mArgs){
		this->func = func;
		this->mArgs = mArgs;
	}  

	Task(callbackFunc2param func, map<string, string>* mArgs){
		this->param2Func = func;
		this->mArgs = mArgs;
	}

	Task(callbackFunc3param func, map<string, string>* mArgs){
		this->param3Func = func;
		this->mArgs = mArgs;
	}

	Task(weakCallbackFunc func, map<string, string>* mArgs){
		this->weakFunc = func;
		this->mArgs = mArgs;
	}
};

class TaskQueue{
private:
	queue<Task> taskQueue;
	mutex queueMutex;
public:
	TaskQueue(){
		this->taskQueue = queue<Task>();
	}
	
	~TaskQueue(){

	}

	// get
	Task getTask(){
		Task task;
		if (!this->taskQueue.empty() && ((*(int*)&this->taskQueue) != 0)){
			queueMutex.lock();
			task = this->taskQueue.front();
			this->taskQueue.pop();
			queueMutex.unlock();
		}

		return task;
	}

	// add
	void addTask(Task task){
		queueMutex.lock();
		this->taskQueue.push(task);
		queueMutex.unlock();
	}

	// add
	void addTask(callbackFunc func, map<string, string>* mArgs){
		queueMutex.lock();
		Task task(func, mArgs);
		this->taskQueue.push(task);
		queueMutex.unlock();
	}

	// add
	void addTask(callbackFunc2param func, map<string, string>* mArgs){
		queueMutex.lock();
		Task task(func, mArgs);
		this->taskQueue.push(task);
		queueMutex.unlock();
	}

	// add
	void addTask(callbackFunc3param func, map<string, string>* mArgs){
		queueMutex.lock();
		Task task(func, mArgs);
		this->taskQueue.push(task);
		queueMutex.unlock();
	}

	// add
	void addTask(weakCallbackFunc func, map<string, string>* mArgs){
		queueMutex.lock();
		Task task(func, mArgs);
		this->taskQueue.push(task);
		queueMutex.unlock();
	}


	// delete
	void deleteTask(){
		queueMutex.lock(); 
		if (!this->taskQueue.empty()){
			this->taskQueue.pop();
		}
		queueMutex.unlock();
	}
	
	DWORD getQueueTaskSize(){
		queueMutex.lock();
		if (*(int*)&this->taskQueue != 0){
			queueMutex.unlock();
			return this->taskQueue.size();
		}
		else{
			queueMutex.unlock();
			return 0;
		}
	}
};

class ThreadPool
{
private:
	TaskQueue* taskQueue; //	线程任务的队列	

private:
	static void manage(void* pool); // 管理线程函数
	static void work(void* pool); // 工作线程函数
	static void produce(void* pool); // 生产线程函数
	//DWORD threadExit();
	
public:
	
	mutex threadMutex;
	DWORD workThreadNum; //		线程池中存活且工作的线程数量
	DWORD aliveThreadNum;//		线程池中存活的线程数量
	DWORD exitThreadNum;
	DWORD dwMinThreadNum;
	DWORD dwMaxThreadNum;
	
	thread* consumeThread; //	消费线程者
	thread managerThread; //	管理线程者

	condition_variable_any conditionVariable;
	BOOL bShutDown = FALSE; // 当前线程池的工作状态，FALSE为运行，为关闭
	
public:
	ThreadPool();
	ThreadPool(DWORD dwMinThreadNum, DWORD dwMaxThreadNum);
	~ThreadPool();
	//////
	
	void addTask(Task task);
	void addTask(callbackFunc func, map<string, string>* mArgs);
	void addTask(callbackFunc2param func, map<string, string>* mArgs);
	void addTask(callbackFunc3param func, map<string, string>* mArgs);
	void addTask(weakCallbackFunc func, map<string, string>* mArgs);
	//////
	Task getTask();
	DWORD getQueueTaskSize();
	DWORD getWorkThreadNum(); // 获取线程池中等待任务的数量
	DWORD getAliveThreadNum(); // 获取线程池中正在运行的数量

};

#endif