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
		std::thread tr(&Communicator::HandleConnection, this, std::ref(client_socket));
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

void Communicator::HandleConnection(SOCKET sock)
{
	// Setting the timeout
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&this->socket_timeout), sizeof(this->socket_timeout));

	while (true)
	{
		std::vector<unsigned char> message = ReceiveSocketMessageAsVector(sock);

		if (IsDirectoryMessage(message))
		{
			RequestHandler handler(message);

			DirResponse response = handler.HandleDirRequest(message);
			
			try { SendData(sock, response.data); }
			catch (std::exception&) {} // No need to do anything
			
			::closesocket(sock);
			return;
		}
		else
		{
			RequestHandler handler(message);
			Request request = handler.HandleRequest(message);
			ServeClient(sock, request);
		}
	}
}

void Communicator::ServeClient(SOCKET incomingSocket, const Request& initialRequest)
{
	SOCKET targetSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if (targetSocket == INVALID_SOCKET)
		throw std::exception("targetSocket creation failed");

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(8200);
	inet_pton(AF_INET, initialRequest.dest_ip.c_str(), &(sa.sin_addr));

	// Setting socket time to get the response
	setsockopt(targetSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&this->socket_timeout), sizeof(this->socket_timeout));

	if (connect(targetSocket, reinterpret_cast<sockaddr*>(&sa), sizeof(sa)) == SOCKET_ERROR) 
		throw std::exception("targetSocket connection failed");

	SendData(targetSocket, initialRequest.data); // Throws an exception on its own

	while (true)
	{
		try
		{
			std::vector<unsigned char> message = ReceiveSocketMessageAsVector(targetSocket);

			// TODO: ENCRYPT THE MESSAGE

			SendData(incomingSocket, message);


			message = ReceiveSocketMessageAsVector(incomingSocket);

			// TODO: DECRYPT THE MESSAGE

			SendData(targetSocket, message);
		}
		catch (std::exception&) { ::closesocket(targetSocket); ::closesocket(incomingSocket); }
	}
}

std::vector<unsigned char> Communicator::ReceiveSocketMessageAsVector(SOCKET sock)
{
	char buffer[max_message_size];
	int len = recv(sock, buffer, max_message_size, NULL);

	// Check connection
	if (len <= 0)
	{
		if (len == SOCKET_ERROR && WSAGetLastError() == WSAETIMEDOUT)
			throw std::exception("Timeout");
		else
			throw std::exception("Connection closed");
	}

	std::vector<unsigned char> response(buffer, buffer + len);

	return response;
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
