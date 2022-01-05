#ifndef THREADPOOL_H_H
#define THREADPOOL_H_H
class ThreadPool
{
private:
	
public:
	ThreadPool();
	~ThreadPool();
	void getThreadId();
	void getWaitThreadCount();
	void run();
};

#endif