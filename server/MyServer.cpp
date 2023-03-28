#include "MyServer.h"

MyServer::MyServer(std::string IP, int port)
{
	this->setIP(IP);
	this->setPort(port);
}

int MyServer::runServer()
{
	std::cout << "----Starting the server----" << std::endl;
	this->handler.startControlThread();
	this->handler.runDistribution();
	this->createSocket();
	this->listenToClient();
	return 0;
}

int MyServer::setPort(int port)
{
	this->port = port;
	return 0;
}

int MyServer::setIP(std::string IP)
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
		std::cout << "Winsock did not start" << std::endl;
		return 1;
	}
	std::cout << "Winsock status: " << this->wsaData.szSystemStatus << std::endl;

	std::cout << "----DLL setup----" << std::endl;
	this->srvSocket = INVALID_SOCKET;
	this->srvSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //address family, socket type (for TCP), protocol
	if (this->srvSocket == INVALID_SOCKET)
	{
		std::cout << "Socket error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 2;
	}
	std::cout << "Socket is good" << std::endl;

	std::cout << "----Bind socket----" << std::endl;
	this->service.sin_family = AF_INET;
	inet_pton(AF_INET, this->IP.c_str(), &(this->service.sin_addr.s_addr));
	this->service.sin_port = htons(port);
	if (::bind(this->srvSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		std::cout << "bind failed: " << WSAGetLastError() << std::endl;
		closesocket(this->srvSocket);
		WSACleanup();
		return 3;
	}
	std::cout << "bound successfully" << std::endl;

	return 0;
}


int MyServer::listenToClient()
{	
	while (true)
	{
		std::cout << "----Listen----" << std::endl;
		if (listen(this->srvSocket, this->max_conns) == SOCKET_ERROR)
			std::cout << "listen error: " << WSAGetLastError() << std::endl;
		std::cout << "listening" << std::endl;

		SOCKET currSocket = accept(this->srvSocket, NULL, NULL);
		if (currSocket == INVALID_SOCKET)
		{
			std::cout << "failed to accept connection: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return 4;
		}
		std::cout << "accepted connection" << std::endl;
		this->handler.addClient(currSocket);
	}
	WSACleanup();
	return 0;
}