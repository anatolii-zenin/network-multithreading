#pragma once
#include "iostream"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>
#include <queue>
#include <chrono>
#include "ThreadManager.h"
#define MAX_CONNS 5

class MyServer
{
public:
	MyServer(std::string IP, int port);
	int runServer();

private:
	int setPort(int port);
	int setIP(std::string IP);
	int createSocket();
	static int talkToClient(SOCKET currSocket, unsigned int clientID, std::queue<std::string> msgQueue);
	int listenToClient();
	static void parseControlCommands(ThreadManager* manager);
	static void distributeMessages(SOCKET currSocket, std::queue<std::string> msgQueue);
	ThreadManager manager;

	SOCKET srvSocket;
	int max_conns = MAX_CONNS;
	WSADATA wsaData;
	int wsaerr;
	int port;
	std::string IP;
	sockaddr_in service;
	std::queue<std::string> messages;
};

