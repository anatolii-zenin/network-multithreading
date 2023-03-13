#include "MyServer.h"

using namespace std;

int MyServer::runServer(string IP, int port)
{
	cout << "----Starting the server----" << endl;
	this->createSocket(IP, port);
	this->listenAndCommunicate();
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

int MyServer::createSocket(string IP, int port)
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
	if (bind(srvSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		cout << "bind failed: " << WSAGetLastError() << endl;
		closesocket(srvSocket);
		WSACleanup();
		return 3;
	}
	cout << "bound successfully" << endl;

	return 0;
}

int MyServer::listenAndCommunicate()
{
	cout << "----Listen----" << endl;
	if (listen(this->srvSocket, 1) == SOCKET_ERROR)
		cout << "listen error: " << WSAGetLastError() << endl;
	cout << "listening" << endl;

	this->acceptSocket = accept(this->srvSocket, NULL, NULL);
	if (this->acceptSocket == INVALID_SOCKET)
	{
		cout << "failed to accept connection: " << WSAGetLastError() << endl;
		WSACleanup();
		return 4;
	}
	cout << "accepted connection" << endl;


	cout << "----Talk to the client----" << endl;
	const int bufferLen = 200;
	char buffer[bufferLen] = "";
	int byteCount = recv(this->acceptSocket, buffer, bufferLen, 0);
	if (byteCount > 0)
		cout << "Message received: " << buffer << endl;
	else
	{
		cout << "Failed to receive the message" << endl;
		WSACleanup();
	}

	char confirmation[bufferLen] = "Server: message received";
	byteCount = send(this->acceptSocket, confirmation, bufferLen, 0);
	if (byteCount > 0)
		cout << "Confirmation sent" << endl;
	else
	{
		cout << "Failed to send the confirmation" << endl;
		WSACleanup();
	}

	cout << "----Disconnect----" << endl;
	system("pause");
	WSACleanup();

	return 0;
}