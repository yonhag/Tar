#include "Communicator.h"
#include "Protocol.h"
#include "Consts.h"
#include "JsonDecoder.h"
#include <thread>
#include <exception>
#include <iostream>
#include <chrono>

Communicator::Communicator(const NetworkHandler& nwh) :
	_nwhandler(nwh)
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
	bindAndListen();
	std::thread sendingThread(&Communicator::SendMessages, this);
	sendingThread.detach();

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

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(server_port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	// Connects between the socket and the configuration (port and etc..)
	if (::bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (::listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << server_port << std::endl;
}

void Communicator::HandleClient(SOCKET sock)
{
	// Recieving the message
	unsigned char buffer[max_message_size];
	int len = recv(sock, reinterpret_cast<char*>(buffer), max_message_size, NULL);
	
	// If connection isn't right
	if (len <= 0) 
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
	
	MessageRequest request = JsonDecoder::DecodeClientMessage(message);

	std::vector<unsigned char> encrypted = this->_nwhandler.EncryptMessage(request);

	this->_messageQueue.push(encrypted);
}

void Communicator::SendMessages()
{
	// Creating a socket with the relays
	SOCKET relaySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (relaySocket = INVALID_SOCKET)
		throw std::exception("Relay socket failed");

	// Socket Specifiers
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(relay_port);
	addr.sin_addr.s_addr = inet_addr(this->_nwhandler.GetFirstRelayIP().c_str());

	// Connecting
	if (connect(relaySocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == INVALID_SOCKET)
		throw std::exception("Relay socket failed");

	while (true)
	{
		// Sleep is queue is empty
		if (this->_messageQueue.empty())
			std::this_thread::sleep_for(std::chrono::seconds(3));

		// Getting the message
		std::vector<unsigned char> message = this->_messageQueue.front();
		this->_messageQueue.pop();
		
		// Sending the message
		if (send(relaySocket, reinterpret_cast<const char*>(message.data()), message.size(), 0) == INVALID_SOCKET)
			throw std::exception("Error while sending message to client");
	}
}
