#include "MyServer.h"

using namespace std;

MyServer::MyServer(string IP, int port)
{
	this->setIP(IP);
	this->setPort(port);
}

int MyServer::runServer()
{
	cout << "----Starting the server----" << endl;
	this->createSocket();
	this->listenToClient();
	return 0;
}

int MyServer::setPort(int port)
{
	this->port = port;
	return 0;
}

int MyServer::setIP(string IP)
{
	this->IP = IP;
	return 0;
}

int MyServer::createSocket()
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
	this->srvSocket = INVALID_SOCKET;
	this->srvSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //address family, socket type (for TCP), protocol
	if (this->srvSocket == INVALID_SOCKET)
	{
		cout << "Socket error: " << WSAGetLastError() << endl;
		WSACleanup();
		return 2;
	}
	cout << "Socket is good" << endl;

	cout << "----Bind socket----" << endl;
	this->service.sin_family = AF_INET;
	inet_pton(AF_INET, this->IP.c_str(), &(this->service.sin_addr.s_addr));
	this->service.sin_port = htons(port);
	if (::bind(this->srvSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		cout << "bind failed: " << WSAGetLastError() << endl;
		closesocket(this->srvSocket);
		WSACleanup();
		return 3;
	}
	cout << "bound successfully" << endl;

	return 0;
}


int MyServer::listenToClient()
{	
	while (true)
	{
		cout << "----Listen----" << endl;
		if (listen(this->srvSocket, this->max_conns) == SOCKET_ERROR)
			cout << "listen error: " << WSAGetLastError() << endl;
		cout << "listening" << endl;

		SOCKET currSocket = accept(this->srvSocket, NULL, NULL);
		if (currSocket == INVALID_SOCKET)
		{
			cout << "failed to accept connection: " << WSAGetLastError() << endl;
			WSACleanup();
			return 4;
		}
		cout << "accepted connection" << endl;
		this->manager.startThread(this->communicate, currSocket);
		//enter control section for the server. for now only one command - /shutdown
		//cout << "sending disconnection signals" << endl;
		// this->threadManager->disconnectAll();
		//cout << "shutting the server down" << endl;
		// this->control->shutdown;
		cout << "thread is running?" << endl;
	}
	WSACleanup();
	return 0;
}

int MyServer::communicate(SOCKET currSocket)
{
	cout << "----Talk to the client----" << endl;
	while (1)
	{
		const int bufferLen = 200;
		char buffer[bufferLen] = "";
		int byteCount = recv(currSocket, buffer, bufferLen, 0);
		if (byteCount > 0)
			cout << "Message received: " << buffer << endl;
		else
		{
			cout << "Failed to receive the message" << endl;
			WSACleanup();
		}

		char confirmation[bufferLen] = "Server: message received";
		byteCount = send(currSocket, confirmation, bufferLen, 0);
		if (byteCount > 0)
			cout << "Confirmation sent" << endl;
		else
		{
			cout << "System: Failed to send the confirmation. Connection lost." << endl;
			break;
		}

		if (string(buffer) == string("/disconnect"))
		{
			cout << "Client has disconnected" << endl;
			char disconnectConfirmation[bufferLen] = "Server: disconnected";
			byteCount = send(currSocket, disconnectConfirmation, bufferLen, 0);
			if (byteCount > 0)
				cout << "Disconnection confirmation sent" << endl;
			else
				cout << "Failed to send disconnection confirmation" << endl;
			break;
		}

		// should send the message to all clients here 
		// use mutex to avoid sending at the same time and potentially losing messages
	}
	cout << "----Disconnect----" << endl;

	return 0;
}