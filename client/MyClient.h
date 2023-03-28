#pragma once
#include "iostream"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include "MessageHandler.h"

using namespace std;

class MyClient
{
public:
	MyClient(string IP, int port);
	int runClient();

private:
	int setPort(int port);
	int setIP(string IP);
	int createSocket();
	int makeConnection();
	MessageHandler handler;

	SOCKET clientSocket;
	WSADATA wsaData;
	int wsaerr;
	int port;
	string IP;
	sockaddr_in service;
};

