#pragma once
#include "iostream"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
using namespace std;

class MyServer
{
public:
	int runServer(string IP, int port);

private:
	int setPort(int port);
	int setIP(string IP);
	int createSocket(string IP, int port);
	int listenAndCommunicate();

	SOCKET srvSocket, acceptSocket;
	WSADATA wsaData;
	int wsaerr;
	int port;
	string IP;
	sockaddr_in service;
};

