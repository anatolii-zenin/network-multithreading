#pragma once
#include <winsock2.h>
#include <queue>
#include <string>
#include <functional>
#include <thread>
#include <chrono>
#include <iostream>
#include <chrono>

class MessageHandler
{
public:
	void receive(SOCKET clientSocket);

private:
	static void receiveMessages(SOCKET currSocket, std::queue<std::string>* msgQueue);
	static void printMessages(std::queue<std::string>* msgQueue);
	void runTask(std::function<void(SOCKET, std::queue<std::string>*)> task, SOCKET socket);
	void runTask(std::function<void(std::queue<std::string>*)> task);
	std::queue<std::string> messages;
};

