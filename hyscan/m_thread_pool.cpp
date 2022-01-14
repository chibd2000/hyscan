#include "m_thread_pool.h"


ThreadPool::ThreadPool(DWORD dwMinThreadNum, DWORD dwMaxThreadNum)
{
	// 创建任务队列TaskQueue
	this->taskQueue = new TaskQueue();
	
	do{
		// 初始化成员属性
		this->dwMinThreadNum = dwMinThreadNum;
		this->dwMaxThreadNum = dwMaxThreadNum;
		this->workThreadNum = 0;
		
		// 创建消费者线程
		this->consumeThread = new thread[this->dwMaxThreadNum]; // 50 0-49 
		RtlZeroMemory(this->consumeThread, sizeof(thread)*this->dwMaxThreadNum);
		
		if (this->consumeThread != NULL){
			for (DWORD i = 0; i<this->dwMaxThreadNum; i++){
				this->consumeThread[i] = thread(ThreadPool::work, this);
				this->aliveThreadNum++;
			}
			cout << "[+] Create ConsumeThread Successed" << endl;
		}else{
			cout << "[-] Create ManagerThread Failed" << endl;
			break;
		}

		// 创建管理者线程
		this->managerThread = thread(ThreadPool::manage, this);
		if (this->consumeThread){
			cout << "[+] Create ManagerThread Successed" << endl;
		}else{
			cout << "[-] Create ConsumeThread Failed" << endl;
			break;
		}
		
	} while (0);

}

ThreadPool::~ThreadPool()
{
	// this->bShutDown = TRUE;
	
	// 还需要对当前线程池中还存在的线程进行销毁
	this->conditionVariable.notify_all();
	
	// 堆内存的释放
	if (this->consumeThread != NULL){
		delete[] this->consumeThread;
	}
	
	if (this->taskQueue != NULL){
		delete this->taskQueue;
		this->taskQueue = NULL;
	}
}

void ThreadPool::addTask(Task task){
	if (this->bShutDown){
		return;
	}
	
	this->taskQueue->addTask(task); 
	// 每次添加完任务的话都可以进行通知下消费者线程
	conditionVariable.notify_one();
}
//////////////////////////////////////
void ThreadPool::addTask(callbackFunc func, map<string, string>* mArgs){
	if (this->bShutDown){
		return;
	}

	this->taskQueue->addTask(func, mArgs);
	// 每次添加完任务的话都可以进行通知下消费者线程
	conditionVariable.notify_one();
}

void ThreadPool::addTask(weakCallbackFunc func, map<string, string>* mArgs){
	if (this->bShutDown){
		return;
	}

	this->taskQueue->addTask(func, mArgs);
	// 每次添加完任务的话都可以进行通知下消费者线程
	conditionVariable.notify_one();
}
//////////////////////////////////////

DWORD ThreadPool::getWorkThreadNum(){
	return this->workThreadNum;
}

DWORD ThreadPool::getAliveThreadNum(){
	return this->aliveThreadNum;
}

DWORD ThreadPool::getQueueTaskSize(){
	return this->taskQueue->getQueueTaskSize();
}

// 消费者函数
void ThreadPool::work(void* pool){
	
	// 消费的话就是取出任务队列中的任务对象来进行消费即可
	ThreadPool* threadPool = (ThreadPool*)pool;
	
	while (1){
		threadPool->threadMutex.lock();
		while (threadPool->taskQueue->getQueueTaskSize() == 0 && !threadPool->bShutDown){
			threadPool->conditionVariable.wait(threadPool->threadMutex);
			// 多余的线程的情况，所以自适应需要自杀掉点线程来减少资源
			if (threadPool->exitThreadNum > 0){
				threadPool->exitThreadNum--;
				
				if (threadPool->aliveThreadNum > threadPool->dwMinThreadNum)
				{
					threadPool->aliveThreadNum--;
					threadPool->threadMutex.unlock();
					for (DWORD i = 0; i < threadPool->dwMaxThreadNum; i++){
						if (threadPool->consumeThread[i].get_id() == this_thread::get_id()){
							RtlZeroMemory(&threadPool->consumeThread[i], sizeof(thread));
							return;
						}
					}
				}
			}
		}

		// 如果此时线程池的bShutDown是True的话，那么则表示已经不需要线程池了，那么当前还存活的线程池都需要进行自杀
		if (threadPool->bShutDown){
			threadPool->aliveThreadNum--;
			threadPool->threadMutex.unlock();
			for (DWORD i = 0; i < threadPool->dwMaxThreadNum; i++){
				if (threadPool->consumeThread[i].get_id() == this_thread::get_id()){
					RtlZeroMemory(&threadPool->consumeThread[i], sizeof(thread));
					return;
				}
			}
		}
		
		// 正常执行
		Task task = threadPool->getTask();
		threadPool->workThreadNum++;
		threadPool->threadMutex.unlock();

		// 		task.func(task.mArgs);
		if (task.mArgs->size() == 1){
			string ipAddr;
			map<string, string>::iterator it = task.mArgs->find("ipAddr");
			if (it != task.mArgs->end())
				ipAddr = it->second;
			task.func(ipAddr);
		}
		else if (task.mArgs->size() == 5){
			string serverName;
			string domainName;
			string domainUsername;
			string domainPassword;
			string personalPassword;
			map<string, string>::iterator it;
			it = task.mArgs->find("serverName");
			if (it != task.mArgs->end())
				serverName = it->second;
			it = task.mArgs->find("domainName");
			if (it != task.mArgs->end())
				domainName = it->second;
			it = task.mArgs->find("domainUsername");
			if (it != task.mArgs->end())
				domainUsername = it->second;
			it = task.mArgs->find("domainPassword");
			if (it != task.mArgs->end())
				domainPassword = it->second;
			it = task.mArgs->find("personalPassword");
			if (it != task.mArgs->end())
				personalPassword = it->second;
			task.weakFunc(serverName, domainName, domainUsername, domainPassword, personalPassword);
		}
		delete task.mArgs;
		task.mArgs = nullptr;
		threadPool->threadMutex.lock();
		threadPool->workThreadNum--;
		threadPool->threadMutex.unlock();
	}
}

Task ThreadPool::getTask(){
	return this->taskQueue->getTask();
}

// 管理者函数

void ThreadPool::manage(void* arg){
	ThreadPool* threadPool = (ThreadPool*)arg;
	DWORD dwQueueSizeCount = 0;
	// 如果线程池没有关闭, 就一直检测
	while (!threadPool->bShutDown)
	{
		// 每隔5s检测一次
		Sleep(1000);
		
		threadPool->threadMutex.lock();
		DWORD dwQueueSize = threadPool->taskQueue->getQueueTaskSize();
		DWORD dwAliveThreadNum = threadPool->getAliveThreadNum();
		DWORD dwWorkThreadNum = threadPool->getWorkThreadNum();
		threadPool->threadMutex.unlock();

		if (dwQueueSize == 0)
			dwQueueSizeCount++;
		if (dwQueueSizeCount == THREAD_CLOSE_NUM)
		{
			threadPool->bShutDown = TRUE;
			threadPool->conditionVariable.notify_all();
			return;
		}
		
		// 每次调整创建/自杀5个线程
		DWORD dwAdjustThreadNum = 5;

#ifdef DEBUG
		cout << "[+] TaskSize		----> " << dwQueueSize << endl;
		cout << "[+] workThreadNum	----> " << threadPool->getWorkThreadNum() << endl;
		cout << "[+] aliveThreadNum ----> " << threadPool->getAliveThreadNum() << endl;
#else
#endif
		// 当前任务个数 > 存活的线程数 && 存活的线程数 < 最大线程个数
		if (dwQueueSize > dwAliveThreadNum && dwAliveThreadNum < threadPool->dwMaxThreadNum)
		{
			threadPool->threadMutex.lock();
			// 这个时候就需要添加线程数量运行
			DWORD dwNum = 0;
			for (DWORD i = 0; i < threadPool->dwMaxThreadNum
				&& dwNum < dwAdjustThreadNum 
				&& dwAliveThreadNum < threadPool->dwMaxThreadNum; i++)
			{
				if (*(DWORD*)&threadPool->consumeThread[i] == 0)
				{
					threadPool->consumeThread[i] = thread(ThreadPool::work, threadPool);
					threadPool->aliveThreadNum++;
					dwNum++;
				}
			}
			threadPool->threadMutex.unlock();
		}
		
		// 销毁多余的线程
		// 工作线程*2 < 存活的线程数目 && 存活的线程数 > 最小线程数量
		if (dwWorkThreadNum * 2 < dwAliveThreadNum && dwAliveThreadNum > threadPool->dwMinThreadNum)
		{
			threadPool->threadMutex.lock();
			threadPool->exitThreadNum = dwAdjustThreadNum;
			threadPool->threadMutex.unlock();
			for (DWORD i = 0; i < dwAdjustThreadNum; i++)
			{
				// 通知消费者线程
				threadPool->conditionVariable.notify_one();
			}
		}
	}
}
