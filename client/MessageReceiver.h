#pragma once
#include <winsock2.h>
#include <queue>
#include <string>
#include <functional>
#include <thread>
#include <chrono>
#include <iostream>
#include <chrono>
#include <mutex>

class MessageReceiver
{
public:
	void receive(SOCKET clientSocket);
	void send(SOCKET clientSocket);

private:
	static void sendMessages(SOCKET clientSocket);
	static void receiveMessages(SOCKET clientSocket, std::queue<std::string>* msgQueue, std::mutex* mutex);	
	static void printMessages(std::queue<std::string>* msgQueue, std::mutex* mutex);
	void startThread(std::function<void(SOCKET)> task, SOCKET socket, bool join = false);
	void startThread(std::function<void(SOCKET, std::queue<std::string>*, std::mutex*)> task, SOCKET socket);
	void startThread(std::function<void(std::queue<std::string>*, std::mutex*)> task);
	std::queue<std::string> messages;
	std::mutex mutex;
};

