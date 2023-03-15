#pragma once
#include "iostream"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>
using namespace std;
#define MAXIMUM_CONNECTIONS 10

class MyServer
{
public:
	MyServer(string IP, int port);
	int runServer();

private:
	int setPort(int port);
	int setIP(string IP);
	int createSocket();
	int makeConnection();
	int communicate(SOCKET currSocket, int ID);
	int listenToClient();

	SOCKET srvSocket;
	SOCKET sockets[MAXIMUM_CONNECTIONS];
	bool IDs[MAXIMUM_CONNECTIONS];
	int nextID = 0;
	WSADATA wsaData;
	int wsaerr;
	int port;
	string IP;
	sockaddr_in service;
};

