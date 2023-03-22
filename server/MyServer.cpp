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
	this->manager.startControlThread(this->parseControlCommands);
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
		this->manager.runTask(this->talkToClient, currSocket, this->messages);
		//enter control section for the server. for now only one command - /shutdown
		//cout << "sending disconnection signals" << endl;
		// this->threadManager->disconnectAll();
		//cout << "shutting the server down" << endl;
		// this->control->shutdown;
	}
	WSACleanup();
	return 0;
}

int MyServer::talkToClient(SOCKET currSocket, unsigned int clientID, std::queue<std::string> msgQueue)
{
	// should use mutex here to avoid races
	cout << "----Talk to the client----" << endl;
	while (1)
	{
		const int bufferLen = 200;
		char buffer[bufferLen] = "";
		int byteCount = recv(currSocket, buffer, bufferLen, 0);
		if (byteCount > 0)
			cout << clientID << ": " << buffer << endl;
		else
		{
			cout << "Failed to receive the message" << endl;
			WSACleanup();
		}
		msgQueue.push(buffer);

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
			cout << "Client " << clientID << " has disconnected" << endl;
			char disconnectConfirmation[bufferLen] = "Server: disconnected";
			byteCount = send(currSocket, disconnectConfirmation, bufferLen, 0);
			if (byteCount > 0)
				cout << "Disconnection confirmation sent" << endl;
			else
				cout << "Failed to send disconnection confirmation" << endl;
			break;
		}
	}
	cout << "----Disconnect----" << endl;

	return 0;
}

void MyServer::parseControlCommands(ThreadManager* manager)
{
	while (1)
	{
		const int bufferLen = 200;
		char buffer[bufferLen] = "";
		cout << " > ";
		cin.getline(buffer, bufferLen);
		if (string(buffer) == string("showclients"))
		{
			manager->listThreads();
		}
	}
}

//// move this to thread manager. also rename the thread manager? or use a different class?
//void MyServer::distributeMessages(SOCKET currSocket, std::queue<std::string> msgQueue)
//{
//	// iterate through the existing clients, wait 5 sec, run the loop again
//	// should use mutex here to avoid races
//	while (1)
//	{
//		const int bufferLen = 200;
//		char buffer[bufferLen] = "";
//		if (msgQueue.size() > 0)
//		{
//			string msg = msgQueue.front();
//			msgQueue.pop();
//			int byteCount = send(currSocket, msg.c_str(), bufferLen, 0);
//		}
//	}
//}