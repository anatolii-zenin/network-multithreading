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
	this->runTask(this->receiveMessages, currSocket);
	return 0;
}

void ClientHandler::runTask(std::function<void(SOCKET, unsigned int, std::queue<std::string>*, std::mutex*)> task, SOCKET socket)
{	
	unsigned int clientID = rand();
	std::thread t = std::thread(task, socket, clientID, &this->messages, &this->mutex);
	this->clientThreads[clientID] = &t;
	this->clientSockets[clientID] = socket;
	t.detach();
}

void ClientHandler::runDistribution()
{
	std::thread t = std::thread(this->distributeMessages, &this->messages, &this->clientSockets, &this->mutex);
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

void ClientHandler::distributeMessages(std::queue<std::string>* msgQueue, std::map<unsigned int, SOCKET>* clientSockets, std::mutex* mutex)
{
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		auto clientIDKeys = std::ranges::views::keys(*clientSockets);
		std::vector<unsigned int> clientIDs{ clientIDKeys.begin(), clientIDKeys.end() };
		if (msgQueue->size() > 0)
		{
			mutex->lock();
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
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					i++;
				}
				if (byteCount == 0)
					std::cout << "communication error: failed to distribute a message to client " << ID << std::endl;
			}
			msgQueue->pop();
			mutex->unlock();
		}

	}
}

void ClientHandler::receiveMessages(SOCKET currSocket, unsigned int clientID, std::queue<std::string>* msgQueue, std::mutex* mutex)
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
		mutex->lock();
		msgQueue->push(std::to_string(clientID) + ": " + std::string(buffer));
		mutex->unlock();

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