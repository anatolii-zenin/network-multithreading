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

void ThreadManager::runTask(std::function<int(SOCKET, unsigned int, std::queue<std::string>)> task, SOCKET socket, std::queue<std::string> msgQueue)
{	
	unsigned int clientID = rand();
	std::thread t = std::thread(task, socket, clientID, msgQueue);
	this->clientThreads[clientID] = &t;
	t.detach();
}

void ThreadManager::runTask(std::function<int(SOCKET, std::queue<std::string>)> task, SOCKET socket, std::queue<std::string> msgQueue)
{

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

void ThreadManager::startControlThread(std::function<void(ThreadManager*)> parseControl)
{
	std::thread t = std::thread(parseControl, this);
	t.detach();
}