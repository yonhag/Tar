#include "Communicator.h"
#include "Protocol.h"
#include "RequestHandler.h"
#include "FileHandler.h"
#include "JsonDeserializer.h"
#include <thread>
#include <exception>
#include <iostream>

const std::chrono::seconds Communicator::timeout = std::chrono::seconds(5);

Communicator::Communicator()
{
	if (this->_serverSocket.listen(Communicator::network_listening_port) != sf::Socket::Done)
		throw std::exception("Invalid server socket");
}

Communicator::~Communicator()
{
	this->_serverSocket.close();
}

void Communicator::RunServer()
{
	std::cout << "Listening on port " << Communicator::network_listening_port << std::endl;

	while (true)
	{
		// Accepting clients
		auto client_socket = std::make_unique<sf::TcpSocket>();
		
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

void Communicator::HandleClient(std::unique_ptr<sf::TcpSocket> sock)
{
	// Recieving the message
	try {
		auto message = this->ReceiveWithTimeout(*sock);
		Response response = RequestHandler::HandleRequest(message);
		Communicator::SendData(*sock, response.data);
	}
	catch (...) { return; }
}

std::vector<unsigned char> Communicator::SendDataThroughNewClientSocket(const std::string& ip, const unsigned short port, const std::vector<unsigned char>& data)
{
	sf::TcpSocket sock;
	if (sock.connect(ip, port) != sf::Socket::Status::Done)
		throw std::exception("Connection failed");

	Communicator::SendData(sock, data);
	return ReceiveWithTimeout(sock);
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
			socket.setBlocking(true);
			throw std::exception("Socket error");
		}
	}
}


bool Communicator::HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time)
{
	return std::chrono::steady_clock::now() - start_time > Communicator::timeout;
}
