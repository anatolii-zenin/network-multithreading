#include "ThreadManager.h"

void ThreadManager::waitAllDone()
{
	std::unique_lock<std::mutex> lock(this->mutex);
	this->conditionVar.wait(lock, [this]()->bool
		{
			return this->clientThreads.empty();
		});

}

ThreadManager::~ThreadManager()
{
	this->waitAllDone();
}

void ThreadManager::startThread(std::function<int(SOCKET, unsigned int)> communicationFunction, SOCKET socket)
{	
	unsigned int clientID = rand();
	std::thread t = std::thread(communicationFunction, socket, clientID);
	this->clientThreads[clientID] = &t;
	t.detach();
}

void ThreadManager::listThreads()
{
	// does not work yet
	auto clientIDKeys = std::ranges::views::keys(this->clientThreads);
	std::vector<unsigned int> clientIDs { clientIDKeys.begin(), clientIDKeys.end() };
	for (auto i : clientIDs)
	{
		std::cout << clientIDs[i] << std::endl;
	}
}

void ThreadManager::runControlThread(std::function<void(ThreadManager*)> parseControl)
{
	std::thread t = std::thread(parseControl, this);
	t.detach();
}