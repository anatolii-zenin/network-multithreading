#pragma once
#include <thread>
#include <functional>
#include <winsock2.h>
#include <mutex>
#include <map>
#include <vector>
#include <ranges>
#include <iostream>
#include <string>
#include <queue>
#include <chrono>
#include <mutex>

class ClientHandler
{
public:
	void runDistribution();
	int addClient(SOCKET currSocket);
	void startControlThread();
	int stopAll();
	void listThreads();
	void waitAllDone();
	~ClientHandler();

private:
	static void distributeMessages(std::queue<std::string>* msgQueue, std::map<unsigned int, SOCKET>* clientSockets, std::mutex* mutex);
	static void receiveMessages(SOCKET currSocket, unsigned int clientID, std::queue<std::string>* msgQueue, std::mutex* mutex);
	void runTask(std::function<void(SOCKET, unsigned int, std::queue<std::string>*, std::mutex*)> task, SOCKET socket);
	static void parseControlCommands(ClientHandler* handler);
	std::map<unsigned int, std::thread*> clientThreads;
	std::map<unsigned int, SOCKET> clientSockets;
	std::queue<std::string> messages;
	std::mutex mutex;
	//std::condition_variable conditionVar;
	//std::mutex mutex;
};

