#pragma once
#include "iostream"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>
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
	static int communicate(SOCKET currSocket);
	int listenToClient();
	ThreadManager manager;

	SOCKET srvSocket;
	int max_conns = MAX_CONNS;
	WSADATA wsaData;
	int wsaerr;
	int port;
	std::string IP;
	sockaddr_in service;
};

