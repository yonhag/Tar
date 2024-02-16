#include "Communicator.h"
#include "Protocol.h"
#include "Consts.h"
#include "JsonDeserializer.h"
#include "JsonSerializer.h"
#include <thread>
#include <exception>
#include <iostream>

const std::chrono::seconds Communicator::timeout = std::chrono::seconds(5);

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

std::vector<unsigned char> Communicator::GetRelays(const Directory& dir, const LoadLevel& ll)
{
	std::vector<unsigned char> request = JsonSerializer::SerializeGetRelaysRequest(ll);

	sf::TcpSocket directorySocket;
	
	directorySocket.connect(dir._ip, dir._port);
	SendData(directorySocket, request);
	
	// Receiving the relays
	// #TODO: change this
	std::vector<unsigned char> relays(128);
	std::size_t recv;
	directorySocket.receive(relays.data(), relays.size(), recv); //ReceiveWithTimeout(directorySocket);

	for (auto& i : relays)
		std::cout << i;

	return relays;
}

void Communicator::HandleClient(std::unique_ptr<sf::TcpSocket> sock)
{
	// Recieving the message
	std::vector<unsigned char> message = ReceiveWithTimeout(*sock);
	
	MessageRequest request = JsonDeserializer::DeserializeClientMessage(message);

	std::vector<unsigned char> encrypted = this->_nwhandler.EncryptMessage(request);

	this->_messageQueue.push(encrypted);
}

void Communicator::SendMessages()
{
	sf::TcpSocket relaySocket;

	if (relaySocket.connect(this->_nwhandler.GetFirstRelayIP(), Communicator::relay_port) != sf::Socket::Status::Done)
	{
		relaySocket.disconnect();
		throw std::exception("Relay socket failed");
	}

	while (true)
	{
		// Sleep if queue is empty
		if (this->_messageQueue.empty())
			std::this_thread::sleep_for(std::chrono::seconds(3));

		// Getting the message
		std::vector<unsigned char> message = this->_messageQueue.front();
		this->_messageQueue.pop();
		
		// Sending the message
		if (SendData(relaySocket, message) != sf::Socket::Status::Done)
			throw std::exception("Error while sending message to client");
	}
}

sf::TcpSocket::Status Communicator::SendData(sf::TcpSocket& socket, const std::vector<unsigned char>& data)
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
			std::cout << status;
			socket.setBlocking(true);
			throw std::exception("Socket error");
		}
	}
}

bool Communicator::HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time)
{
	return std::chrono::steady_clock::now() - start_time > Communicator::timeout;
}
