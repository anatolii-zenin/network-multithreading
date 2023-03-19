#include "ThreadManager.h"

void ThreadManager::waitAllDone()
{
	std::unique_lock<std::mutex> lock(this->mutex);
	this->conditionVar.wait(lock, [this]()->bool
		{
			return this->threadList.empty();
		});

}

ThreadManager::~ThreadManager()
{
	this->waitAllDone();
}

void ThreadManager::startThread(std::function<int(SOCKET*)> fun, SOCKET* sock)
{
	std::thread t = std::thread(fun, sock);
	//fun(sock);
	//this->threadList.Append();
}