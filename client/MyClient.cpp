#include "MyClient.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <tchar.h>
using namespace std;

MyClient::MyClient(string IP, int port)
{
	setPort(port);
	setIP(IP);
}

int MyClient::runClient()
{
	cout << "----Starting the server----" << endl;
	this->createSocket();
	this->makeConnection();
	return 0;
}

int MyClient::createSocket()
{
	WORD wVersionRequested = MAKEWORD(2, 2);

	this->wsaerr = WSAStartup(wVersionRequested, &(this->wsaData));
	if (this->wsaerr != 0)
	{
		cout << "Winsock did not start" << endl;
		return 1;
	}
	cout << "Winsock status: " << this->wsaData.szSystemStatus << endl;

	cout << "----DLL setup----" << endl;
	this->clientSocket = INVALID_SOCKET;
	this->clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //address family, socket type (for TCP), protocol
	if (this->clientSocket == INVALID_SOCKET)
	{
		cout << "Socket error: " << WSAGetLastError() << endl;
		WSACleanup();
		return 2;
	}
	cout << "Socket is good" << endl;
	return 0;
}

int MyClient::setPort(int port)
{
	this->port = port;
	return 0;
}

int MyClient::setIP(string IP)
{
	this->IP = IP;
	return 0;
}

int MyClient::makeConnection()
{
	this->service.sin_family = AF_INET;
	inet_pton(AF_INET, this->IP.c_str(), &(this->service.sin_addr.s_addr));
	this->service.sin_port = htons(port);
	if (connect(this->clientSocket, (SOCKADDR*)&(this->service), sizeof(this->service)) == SOCKET_ERROR)
	{
		cout << "Client: failed to connect: " << endl;
		WSACleanup();
		return 1;
	}
	cout << "Connected to the server" << endl;
	this->handler.receive(this->clientSocket);
	this->handler.send(this->clientSocket);
	return 0;
}

