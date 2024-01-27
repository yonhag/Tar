#include "Communicator.h"
#include "Protocol.h"
#include "Consts.h"
#include "JsonDeserializer.h"
#include <thread>
#include <exception>
#include <iostream>

Communicator::Communicator(const NetworkHandler& nwh) :
	_nwhandler(nwh)
{
	this->_serverSocket.listen(server_port);
}

Communicator::~Communicator()
{
	this->_serverSocket.close();
}

void Communicator::RunServer()
{
	std::cout << "Listening on port " << this->server_port << std::endl;

	std::thread sendingThread(&Communicator::SendMessages, this);
	sendingThread.detach();

	while (true)
	{
		// Accepting clients
		auto clientSocket = std::make_unique<sf::TcpSocket>();
		if (this->_serverSocket.accept(*clientSocket) == sf::Socket::Status::Done);

		std::cout << "Accepting client..." << std::endl;

		// Detaching client handling to a different thread
		this->HandleClient(std::move(clientSocket));
	}

}

std::vector<unsigned char> Communicator::GetRelays()
{
	return std::vector<unsigned char>();
}

void Communicator::HandleClient(std::unique_ptr<sf::TcpSocket> sock)
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
	
	MessageRequest request = JsonDeserializer::DeserializeClientMessage(message);

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
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(this->relay_port);

	PCWSTR ip = NetworkHandler::StringToPCWSTR(this->_nwhandler.GetFirstRelayIP());
	if (InetPton(AF_INET, ip, &(serverAddress.sin_addr)) != 1)
	{
		closesocket(relaySocket);
		throw std::exception("Relay socket failed");
	}

	// Connecting
	if (connect(relaySocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == INVALID_SOCKET)
	{
		closesocket(relaySocket);
		throw std::exception("Relay socket failed");
	}

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
			if (Communicator::HasTimeoutPassed(start_time))
			{
				socket.setBlocking(true);
				throw std::exception("Timeout passed");
			}
			// Sleeping for 0.5s to avoid excessive CPU usage
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		else // Socket error
		{
			socket.setBlocking(true);
			throw std::exception("Socket error");
		}
	}
}

bool Communicator::HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time)
{
	return std::chrono::steady_clock::now() - start_time > Communicator::timeout;
}
