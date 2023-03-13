#pragma once
#include "iostream"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
using namespace std;

class MyClient
{
public:
	int runClient(string IP, int port);

private:
	int setPort(int port);
	int setIP(string IP);
	int createSocket(string IP, int port);
	int communicate();

	SOCKET clientSocket;
	WSADATA wsaData;
	int wsaerr;
	int port;
	string IP;
	sockaddr_in service;
};

