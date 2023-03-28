#include "MessageHandler.h"

void MessageHandler::receive(SOCKET clientSocket)
{
	this->startThread(this->receiveMessages, clientSocket);
	this->startThread(this->printMessages);
}

void MessageHandler::receiveMessages(SOCKET clientSocket, std::queue<std::string>* msgQueue, std::mutex* mutex)
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
		mutex->lock();
		msgQueue->push(buffer);
		mutex->unlock();
	}
}

void MessageHandler::send(SOCKET clientSocket)
{
	this->startThread(this->sendMessages, clientSocket, true);
}

void MessageHandler::startThread(std::function<void(SOCKET, std::queue<std::string>*, std::mutex*)> task, SOCKET socket)
{
	std::thread t = std::thread(task, socket, &this->messages, &this->mutex);
	t.detach();
}

void MessageHandler::startThread(std::function<void(SOCKET)> task, SOCKET socket, bool join)
{
	std::thread t = std::thread(task, socket);
	if (join)
		t.join();
	else
		t.detach();
}

void MessageHandler::startThread(std::function<void(std::queue<std::string>*, std::mutex*)> task)
{
	std::thread t = std::thread(task, &this->messages, &this->mutex);
	t.detach();
}

void MessageHandler::printMessages(std::queue<std::string>* msgQueue, std::mutex* mutex)
{
	while (1)
	{
		if (msgQueue->size() > 0)
		{
			mutex->lock();
			std::cout << msgQueue->front() << std::endl;
			msgQueue->pop();
			mutex->unlock();
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}


void MessageHandler::sendMessages(SOCKET clientSocket)
{
	while (1)
	{
		const int bufferLen = 200;
		char buffer[bufferLen] = "";
		std::cin.getline(buffer, bufferLen);
		int byteCount = ::send(clientSocket, buffer, bufferLen, 0);
		if (byteCount == 0)
		{
			std::cout << "Failure" << std::endl;
			WSACleanup();
		}

		if (std::string(buffer) == std::string("/disconnect"))
		{
			std::cout << "Disconnecting from the server" << std::endl;
			char dsConfirmation[bufferLen] = "";
			byteCount = recv(clientSocket, dsConfirmation, bufferLen, 0);
			if (dsConfirmation == "Server: disconnected")
				std::cout << "Disconnection confirmed" << std::endl;
			else
			{
				std::cout << "Failed to receive the disconnection confirmation" << std::endl;
			}
			break;
		}
	}

	std::cout << "----Close the socket and disconnect----" << std::endl;
	system("pause");
	WSACleanup();
}