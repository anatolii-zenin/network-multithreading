#pragma once
#include <thread>
#include <list> 
#include <functional>
#include <winsock2.h>
#include <mutex>
#include <map>
#include <vector>
#include <ranges>
#include <iostream>
#include <queue>

class ThreadManager
{
public:
	void runTask(std::function<int(SOCKET, std::queue<std::string>)> task, SOCKET socket, std::queue<std::string> msgQueue);
	void runTask(std::function<int(SOCKET, unsigned int, std::queue<std::string>)> task, SOCKET socket, std::queue<std::string> msgQueue);
	void startControlThread(std::function<void(ThreadManager*)> parseControl);
	int stopAll();
	void listThreads();
	void waitAllDone();
	~ThreadManager();

private:
	std::map<unsigned int, std::thread*> clientThreads;

	std::condition_variable conditionVar;
	std::mutex mutex;
};

