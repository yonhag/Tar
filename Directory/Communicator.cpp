#include "Communicator.h"
#include "Protocol.h"
#include "RequestHandler.h"
#include "FileHandler.h"
#include "JsonDeserializer.h"
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
		auto client_socket = std::unique_ptr<sf::TcpSocket>();

		// If connection is fine
		if (this->_serverSocket.accept(*client_socket) == sf::Socket::Status::Done)
		{
			std::cout << "Accepting client..." << std::endl;

			// Detaching client handling to a different thread
			std::thread tr(&Communicator::HandleClient, this, std::move(client_socket));
			tr.detach();
		}
		else
			std::cout << "Problems with socket connection" << std::endl;
	}
}

Response Communicator::SendRelayConnectionRequest(const Relay& relay, const std::vector<unsigned char>& request)
{
	Response response;
	response.data = Communicator::SendDataThroughNewClientSocket(relay.ip, Communicator::network_listening_port, request);
	return response;
}

unsigned int Communicator::UpdateOtherDirectories(const Request& relayRequest)
{
	FileHandler fh;
	unsigned int directoriesUpdated = 0;

	while (true)
	{
		std::string ip = fh.GetNextDirectoryIP();
		if (ip == "")
			break;

		std::vector<unsigned char> response = SendDataThroughNewClientSocket(ip, Communicator::network_listening_port, relayRequest.data);
		
		if (JsonDeserializer::DeserializeUpdateDirectoriesResponse(response))
			directoriesUpdated++;
	}
	return directoriesUpdated;
}

void Communicator::BindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(network_listening_port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	// Connects between the socket and the configuration (port and etc..)
	if (::bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (::listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << network_listening_port << std::endl;
}

void Communicator::HandleClient(std::unique_ptr<sf::TcpSocket> sock)
{
	// Recieving the message
	unsigned char buffer[Communicator::max_message_size];
	int len = recv(sock, reinterpret_cast<char*>(buffer), Communicator::max_message_size, NULL);

	if (len <= 0) // If connection isn't right
	{
		if (len == 0)
			// Client closed the socket
			std::cout << "Client closed the connection" << std::endl;
		else
			// Error occurred, handle it accordingly
			std::cout << "Error in receiving data from client" << std::endl;
		return;
	}

	// Dealing with the message
	std::vector<unsigned char> message(buffer, buffer + len);
	
	Response response = RequestHandler::HandleRequest(message);
	
	SendData(sock, response.data);
}

sf::TcpSocket::Status Communicator::SendData(sf::TcpSocket& socket, const std::vector<unsigned char>& data) const
{
	return socket.send(data.data(), data.size());
}

std::vector<unsigned char> Communicator::ReceiveWithTimeout(sf::TcpSocket& socket)
{
	socket.setBlocking(false);
	auto start_time = std::chrono::steady_clock::now();
	std::size_t received;
	std::vector<unsigned char> buffer(max_message_size);

	while (true)
	{
		sf::Socket::Status status = socket.receive(buffer.data(), buffer.size(), received);
		if (status == sf::Socket::Done) // Received data
		{
			socket.setBlocking(true);
			return std::vector<unsigned char>(buffer.begin(), buffer.begin() + received);
		}
		else if (status == sf::Socket::NotReady) // No data yet
		{
			if (this->HasTimeoutPassed(start_time))
			{
				socket.setBlocking(true);
				throw std::exception("Timeout passed");
			}
			// Sleeping for 0.5s to avoid excessive CPU usage
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		else // Socket error
		{
			std::cout << status;
			socket.setBlocking(true);
			throw std::exception("Socket Error");
		}
	}
}