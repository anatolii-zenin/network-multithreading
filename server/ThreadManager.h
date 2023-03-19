#pragma once
#include <thread>
#include <list> 
#include <functional>
#include <winsock2.h>
#include <mutex>

class ThreadManager
{
public:
	void startThread(std::function<int(SOCKET*)> fun, SOCKET* sock);
	int stopAll();
	void waitAllDone();
	typedef std::list<std::thread>::iterator iterator;

	~ThreadManager();

private:
	std::condition_variable conditionVar;
	std::list<std::thread> threadList;
	std::mutex mutex;
};

