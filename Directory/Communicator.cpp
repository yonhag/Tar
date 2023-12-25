#include "Communicator.h"
#include "Protocol.h"
#include "RequestHandler.h"
#include <Ws2tcpip.h>
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

Response Communicator::SendRelayConnectionRequest(const Relay& relay, const std::vector<unsigned char>& request)
{
	// Creating the socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		throw std::exception("Invalid Socket Creation");

	// Setting the socket
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(Communicator::relay_port);

	if (InetPton(AF_INET, StringToPCWSTR(relay.ip), &serverAddress.sin_addr) != 1)
	{
		closesocket(sock);
		throw std::exception("Socket creation failed");
	}

	// Connecting
	if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		throw std::exception("Connection Failed");

	// Sending
	if (send(sock, reinterpret_cast<const char*>(request.data()), request.size(), 0) == INVALID_SOCKET)
		throw std::exception("Sending failed");

	// Receiving
	char buffer[Communicator::max_message_size];
	int len = recv(sock, buffer, sizeof(buffer), NULL);

	if (len <= 0)
		throw std::exception("Error while receiving");

	// Transferring
	Response response;
	std::vector<unsigned char> receivedResponse(reinterpret_cast<unsigned char>(buffer));

	response.data = receivedResponse;

	return response;
}

int Communicator::UpdateOtherDirectories(const Relay& newRelay)
{
	while (true)
	{
		std::string ip = ;
	}
	return 0;
}

void Communicator::BindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(relay_port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	// Connects between the socket and the configuration (port and etc..)
	if (::bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (::listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << relay_port << std::endl;
}

void Communicator::HandleClient(SOCKET sock)
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

	Response response = RequestHandler::HandleRequest(message);
	SendData(sock, response.data);
}

void Communicator::SendData(SOCKET sock, const std::vector<unsigned char>& data)
{
	if (send(sock, reinterpret_cast<const char*>(data.data()), data.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}

PCWSTR Communicator::StringToPCWSTR(const std::string& str)
{
	// Convert std::string to wide string using the system's default code page
	int bufferSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	if (bufferSize == 0)
		throw std::exception("Error in StringToPCWSTR");

	std::wstring wideBuffer(bufferSize, L'\0');

	// Convert std::string to wide string
	if (MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wideBuffer[0], bufferSize) == 0)
		throw std::exception("Error in StringToPCWSTR");

	return wideBuffer.c_str();
}
