#include "MessageHandler.h"

void MessageHandler::receive(SOCKET clientSocket)
{
	this->runTask(this->receiveMessages, clientSocket);
	this->runTask(this->printMessages);
}

void MessageHandler::receiveMessages(SOCKET clientSocket, std::queue<std::string>* msgQueue)
{
	while (1)
	{
		const int bufferLen = 200;
		char buffer[bufferLen] = "";
		int byteCount = recv(clientSocket, buffer, bufferLen, 0);
		if (byteCount == 0)
		{
			std::cout << "Failed to receive the message. Connection lost." << std::endl;
			WSACleanup();
			break;
		}
		msgQueue->push(buffer);
	}
}

void MessageHandler::runTask(std::function<void(SOCKET, std::queue<std::string>*)> task, SOCKET socket)
{
	std::thread t = std::thread(task, socket, &this->messages);
	t.detach();
}

void MessageHandler::runTask(std::function<void(std::queue<std::string>*)> task)
{
	std::thread t = std::thread(task, &this->messages);
	t.detach();
}

void MessageHandler::printMessages(std::queue<std::string>* msgQueue)
{
	while (1)
	{
		if (msgQueue->size() > 0)
		{
			std::cout << msgQueue->front() << std::endl;
			msgQueue->pop();
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}