#include "ClientHandler.h"

void ClientHandler::waitAllDone()
{

}

ClientHandler::~ClientHandler()
{
	this->waitAllDone();
}

int ClientHandler::addClient(SOCKET currSocket)
{
	this->runTask(this->receiveFromClient, currSocket);
	return 0;
}

void ClientHandler::runTask(std::function<void(SOCKET, unsigned int, std::queue<std::string>*)> task, SOCKET socket)
{	
	unsigned int clientID = rand();
	std::thread t = std::thread(task, socket, clientID, &this->messages);
	this->clientThreads[clientID] = &t;
	this->clientSockets[clientID] = socket;
	t.detach();
}

void ClientHandler::runDistribution()
{
	std::thread t = std::thread(this->distributeMessages, &this->messages, &this->clientSockets);
	t.detach();
}

void ClientHandler::listThreads()
{
	auto clientIDKeys = std::ranges::views::keys(this->clientThreads);
	std::vector<unsigned int> clientIDs { clientIDKeys.begin(), clientIDKeys.end() };
	for (auto ID : clientIDs)
	{
		std::cout << ID << std::endl;
	}
}

void ClientHandler::startControlThread()
{
	std::thread t = std::thread(this->parseControlCommands, this);
	t.detach();
}

void ClientHandler::distributeMessages(std::queue<std::string>* msgQueue, std::map<unsigned int, SOCKET>* clientSockets)
{
	// iterate through the existing clients, wait 1 sec, run the loop again
	// should use mutex here to avoid races
	bool sentFlag = false;
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		auto clientIDKeys = std::ranges::views::keys(*clientSockets);
		std::vector<unsigned int> clientIDs{ clientIDKeys.begin(), clientIDKeys.end() };
		if (msgQueue->size() > 0)
		{
			const int bufferLen = 200;
			char buffer[bufferLen] = "";
			for (auto ID : clientIDs)
			{
				std::string msg = msgQueue->front();
				int byteCount = 0;
				int i = 0;
				while (byteCount == 0 && i < 5 )
				{
					byteCount = send(clientSockets->at(ID), msg.c_str(), bufferLen, 0);
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					i++;
				}
				sentFlag = true;
			}
		}
		if (sentFlag)
		{
			msgQueue->pop();
			sentFlag = false;
		}
	}
}

void ClientHandler::receiveFromClient(SOCKET currSocket, unsigned int clientID, std::queue<std::string>* msgQueue)
{
	// should use mutex here to avoid races
	std::cout << "----Talk to the client----" << std::endl;
	while (1)
	{
		const int bufferLen = 200;
		char buffer[bufferLen] = "";
		int byteCount = recv(currSocket, buffer, bufferLen, 0);
		if (byteCount > 0)
			std::cout << clientID << ": " << buffer << std::endl;
		else
		{
			std::cout << "Failed to receive the message" << std::endl;
			WSACleanup();
		}
		msgQueue->push(std::to_string(clientID) + ": " + std::string(buffer));

		//char confirmation[bufferLen] = "Server: message received";
		//byteCount = send(currSocket, confirmation, bufferLen, 0);
		//if (byteCount > 0)
		//	std::cout << "Confirmation sent" << std::endl;
		//else
		//{
		//	std::cout << "System: Failed to send the confirmation. Connection lost." << std::endl;
		//	break;
		//}

		if (std::string(buffer) == std::string("/disconnect"))
		{
			std::cout << "Client " << clientID << " has disconnected" << std::endl;
			char disconnectConfirmation[bufferLen] = "Server: disconnected";
			byteCount = send(currSocket, disconnectConfirmation, bufferLen, 0);
			if (byteCount > 0)
				std::cout << "Disconnection confirmation sent" << std::endl;
			else
				std::cout << "Failed to send disconnection confirmation" << std::endl;
			break;
		}
	}
	std::cout << "----Disconnect----" << std::endl;
}

void ClientHandler::parseControlCommands(ClientHandler* handler)
{
	while (1)
	{
		const int bufferLen = 200;
		char buffer[bufferLen] = "";
		std::cout << " > ";
		std::cin.getline(buffer, bufferLen);
		if (std::string(buffer) == std::string("showclients"))
		{
			handler->listThreads();
		}
	}
}