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
	this->communicate();
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
	cout << "Client connected" << endl;
	this->receiver.receive(this->clientSocket);
	return 0;
}

int MyClient::communicate()
{
	cout << "----Talk to the server----" << endl;
	while (1)
	{
		const int bufferLen = 200;
		char buffer[bufferLen] = "";
		cout << "Type your message > ";
		cin.getline(buffer, bufferLen);
		int byteCount = send(this->clientSocket, buffer, bufferLen, 0);
		if (byteCount == 0)
		{
			cout << "Failure" << endl;
			WSACleanup();
		}

		if (string(buffer) == string("/disconnect"))
		{
			cout << "Disconnecting from the server" << endl;
			char dsConfirmation[bufferLen] = "";
			byteCount = recv(clientSocket, dsConfirmation, bufferLen, 0);
			if (dsConfirmation == "Server: disconnected")
				cout << "Disconnection confirmed" << endl;
			else
			{
				cout << "Failed to receive the disconnection confirmation" << endl;
			}
			break;
		}

		//char confirmation[bufferLen] = "";
		//byteCount = recv(clientSocket, confirmation, bufferLen, 0);
		//if (byteCount > 0)
		//	cout << confirmation << endl;
		//else
		//{
		//	cout << "Failed to receive the confirmation" << endl;
		//}
	}

	cout << "----Close the socket and disconnect----" << endl;
	system("pause");
	WSACleanup();
	return 0;
}
