#include "Communicator.h"
#include "Protocol.h"
#include <thread>
#include <exception>
#include <iostream>

Communicator::Communicator()
{
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSocket == INVALID_SOCKET)
		throw std::exception("Invalid Socket Creation");
}

Communicator::~Communicator()
{
	try
	{
		::closesocket(_serverSocket);
	}
	catch (...) {}
}

void Communicator::RunServer()
{
	BindAndListen();

	while (true)
	{
		// Accepting clients
		SOCKET client_socket = accept(this->_serverSocket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		std::cout << "Accepting client..." << std::endl;

		// Detaching client handling to a different thread
		std::thread tr(&Communicator::HandleClient, this, std::ref(client_socket));
		tr.detach();
	}

}

void Communicator::BindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	// Connects between the socket and the configuration (port and etc..)
	if (::bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (::listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;
}

void Communicator::HandleClient(SOCKET sock)
{
	try
	{
		while (true)
		{
			// Recieving the message
			unsigned char buffer[max_message_size];
			int len = recv(sock, reinterpret_cast<char*>(buffer), max_message_size, NULL);

			if (len <= 0) // If connection isn't right
			{
				if (len == 0)
				{
					// Client closed the socket
					std::cout << "Client closed the connection" << std::endl;
				}
				else
				{
					// Error occurred, handle it accordingly
					std::cout << "Error in receiving data from client" << std::endl;
				}
				return;
			}

			// Dealing with the message
			std::vector<unsigned char> message(buffer, buffer + len);

			if (IsDirectoryMessage(message))
			{
				RequestHandler handler(message);
				DirResponse response = handler.HandleDirRequest(message);
				SendData(sock, response.data);
				return;
			}
			else
			{
				// Move to different function?
				Request request = handler.HandleRequest(message);
				SendData(sock, request.data);
			}
		}
	}
	catch (...) {  } // Socket closed
}

bool Communicator::IsDirectoryMessage(const std::vector<unsigned char>& message)
{
	if (message[0] == 'D' && message[1] == 'I' && message[2] == 'R')
		return true;
	return false;
}

void Communicator::SendData(SOCKET sock, const std::vector<unsigned char>& data)
{
	if (send(sock, reinterpret_cast<const char*>(data.data()), data.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}
