#include "MyClient.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <tchar.h>
using namespace std;

int MyClient::runClient(string IP, int port)
{
	cout << "----Starting the server----" << endl;
	this->createSocket(IP, port);
	this->communicate();
	return 0;
}

int MyClient::createSocket(string IP, int port)
{
	this->setIP(IP);
	this->setPort(port);
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

int MyClient::communicate()
{
	this->service.sin_family = AF_INET;
	inet_pton(AF_INET, this->IP.c_str(), &(this->service.sin_addr.s_addr));
	this->service.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&(this->service), sizeof(this->service)) == SOCKET_ERROR)
	{
		cout << "Client: failed to connect: " << endl;
		WSACleanup();
		return 1;
	}
	cout << "Client connected" << endl;

	cout << "----Talk to the server----" << endl;
	const int bufferLen = 200;
	char buffer[bufferLen] = "";
	cout << "Type your message > ";
	cin.getline(buffer, bufferLen);
	int byteCount = send(clientSocket, buffer, bufferLen, 0);
	if (byteCount > 0)
		cout << "Message sent" << endl;
	else
	{
		cout << "Failure" << endl;
		WSACleanup;
	}

	char confirmation[bufferLen] = "";
	byteCount = recv(clientSocket, confirmation, bufferLen, 0);
	if (byteCount > 0)
		cout << confirmation << endl;
	else
	{
		cout << "Failed to receive the confirmation" << endl;
		WSACleanup();
	}
	cout << "----Close the socket and disconnect----" << endl;
	system("pause");
	WSACleanup();
}