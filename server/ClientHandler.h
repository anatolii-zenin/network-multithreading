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
	static void distributeMessages(std::queue<std::string>* msgQueue, std::map<unsigned int, SOCKET>* clientSockets);
	static void receiveFromClient(SOCKET currSocket, unsigned int clientID, std::queue<std::string>* msgQueue);
	void runTask(std::function<void(SOCKET, std::queue<std::string>*)> task, SOCKET socket);
	void runTask(std::function<void(SOCKET, unsigned int, std::queue<std::string>*)> task, SOCKET socket);
	static void parseControlCommands(ClientHandler* handler);
	std::map<unsigned int, std::thread*> clientThreads;
	std::map<unsigned int, SOCKET> clientSockets;
	std::queue<std::string> messages;
	//std::condition_variable conditionVar;
	//std::mutex mutex;
};

