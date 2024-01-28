#include "Communicator.h"
#include "Protocol.h"
#include "Request.h"
#include <thread>
#include <exception>
#include <iostream>
#include <memory>
#include "SFML/System.hpp"

const std::chrono::seconds Communicator::timeout = std::chrono::seconds(5);

Communicator::Communicator()
{
	if (this->_serverSocket.listen(port) != sf::Socket::Status::Done)
		throw std::runtime_error("Invalid server socket");
}

Communicator::~Communicator()
{
	this->_serverSocket.close();
}

void Communicator::RunServer()
{
	std::cout << "Listening on port " << this->port << std::endl;

	while (true)
	{
		// Accepting clients
		auto client_socket = std::make_unique<sf::TcpSocket>();

		// If connection is fine
		if (this->_serverSocket.accept(*client_socket) == sf::Socket::Status::Done)
		{
			std::cout << "Accepting client..." << std::endl;

			// Detaching client handling to a different thread
			std::thread tr(&Communicator::HandleConnection, this, std::move(client_socket));
			tr.detach();
		}
		else
			std::cout << "Problems with socket connection" << std::endl;
	}

}

void Communicator::HandleConnection(std::unique_ptr<sf::TcpSocket> socket)
{
	while (true)
	{
		// Receiving data
		std::vector<unsigned char> data = ReceiveWithTimeout(*socket);

		if (IsDirectoryMessage(data))
		{
			RequestHandler handler(data);

			DirResponse response = handler.HandleDirRequest(data);
			
			this->SendData(*socket, response.data);

			return;
		}
		else
		{
			RequestHandler handler(data);
			Request request = handler.HandleRequest(data);
			ServeClient(*socket, request);
		}
	}
}

void Communicator::ServeClient(sf::TcpSocket& incomingSocket, const Request& initialRequest)
{
	sf::TcpSocket targetSocket;
	if (targetSocket.connect(sf::IpAddress(initialRequest.dest_ip), this->port) != sf::Socket::Status::Done)
		return;

	SendData(targetSocket, initialRequest.data); // Throws an exception on its own

	// While loop closes on its own when RecieveWithTimeout closes without a connection made - exception thrown.
	try
	{
		while (true)
		{
			std::vector<unsigned char> message = ReceiveWithTimeout(targetSocket);

			// TODO: ENCRYPT THE MESSAGE

			SendData(incomingSocket, message);

			message = ReceiveWithTimeout(incomingSocket);

			// TODO: DECRYPT THE MESSAGE

			SendData(targetSocket, message);
		}
	}
	catch (...) {} // No need to cleanup - SFML destructor will.
}

bool Communicator::IsDirectoryMessage(const std::vector<unsigned char>& message)
{
	if (message[0] == 'D' && message[1] == 'I' && message[2] == 'R')
		return true;
	return false;
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
		if (status == sf::Socket::Status::Done) // Received data
		{
			socket.setBlocking(true);
			return std::vector<unsigned char>(buffer.begin(), buffer.begin() + received);
		}
		else if (status == sf::Socket::Status::NotReady) // No data yet
		{
			if (Communicator::HasTimeoutPassed(start_time))
			{
				socket.setBlocking(true);
				throw std::runtime_error("Timeout passed");
			}
			// Sleeping for 0.5s to avoid excessive CPU usage
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		else // Socket error
		{
			socket.setBlocking(true);
			throw std::runtime_error("Socket error");
		}
	}
}


bool Communicator::HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time)
{
	return std::chrono::steady_clock::now() - start_time > Communicator::timeout;
}