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

class ThreadManager
{
public:
	void startThread(std::function<int(SOCKET, unsigned int)> communicationFunction, SOCKET socket);
	int stopAll();
	void listThreads();
	void waitAllDone();
	void runControlThread(std::function<void(ThreadManager*)> parseControl);
	~ThreadManager();

private:
	std::map<unsigned int, std::thread*> clientThreads;

	std::condition_variable conditionVar;
	std::mutex mutex;
};

