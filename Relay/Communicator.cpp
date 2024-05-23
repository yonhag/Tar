#include "Communicator.h"
#include "Protocol.h"
#include "Request.h"
#include <thread>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include "Deserializer.h"
#include "Serializer.h"
#include "SFML/System.hpp"
#include "FileHandler.h"

const std::chrono::seconds Communicator::timeout = std::chrono::seconds(50);

Communicator::Communicator(unsigned short port) : _listening_port(port)
{
	FileHandler fh;
	while (true)
	{
		Directory dir = fh.GetNextDirectory();
		if (dir.ip == "")
			throw std::runtime_error("Unable to connect to directory");
		if (ConnectToDirectory(dir))
			break;	
	}

	if (this->_serverSocket.listen(_listening_port) != sf::Socket::Status::Done)
		throw std::runtime_error("Invalid server socket");
}

Communicator::~Communicator()
{
	this->_serverSocket.close();
}

void Communicator::RunServer()
{
	std::cout << "Listening on port " << this->_listening_port << std::endl;

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

		for (auto& i : data)
			std::cout << i;
		std::cout << std::endl;

		if (IsDirectoryMessage(data))
		{
			std::cout << "Dir message" << std::endl;

			RequestHandler handler;

			DirResponse response = handler.HandleDirRequest(data);
			
			std::cout << "Sending data: ";
			for (auto& i : response.data)
				std::cout << i;
			std::cout << std::endl;

			this->SendData(*socket, response.data);

			return;
		}
		else
		{
			RequestHandler handler;
			Request request = handler.HandleRequest(data);
			ServeClient(*socket, request);
		}
	}
}

void Communicator::ServeClient(sf::TcpSocket& incomingSocket, const Request& initialRequest)
{
	sf::TcpSocket targetSocket;
	try
	{
		if (targetSocket.connect(StringToIP(initialRequest.dest_ip), this->client_port) != sf::Socket::Status::Done)
			return;
	}
	catch (std::exception& e) { std::cout << e.what() << std::endl; return; }

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

bool Communicator::ConnectToDirectory(const Directory& dir)
{

	sf::TcpSocket sock;

	std::cout << "Connecting to " << StringToIP(dir.ip) << ":" << dir.port << std::endl;
	
	if (sock.connect(StringToIP(dir.ip), dir.port) != sf::Socket::Status::Done)
		return false;
	
	std::cout << "Connected" << std::endl;

	AES aes = SendRSAHandshake(sock);

	std::cout << "AES DONE";

	auto EncMsg = aes.EncryptCBC(Serializer::SerializeDirectoryConnectionRequest(sf::IpAddress::getLocalAddress()->toString(), 500, this->_listening_port));

	std::cout << "Message: ";
	for (auto& i : aes.DecryptCBC(EncMsg))
		std::cout << i;
	std::cout << std::endl;

	if (SendData(sock, EncMsg) != sf::Socket::Status::Done) // TODO: Change this to my actual ip and bandwidth)
		std::cout << "Error in sending data";

	std::cout << "data sent" << std::endl;

	if (Deserializer::DeserializeDirectoryConnectionResponse(ReceiveWithTimeout(sock)))
		return true;
	return false;
}

AES Communicator::SendRSAHandshake(sf::TcpSocket& socket)
{
	RSA rsa;
	std::cout << "RSAKey: " << rsa.GetPublicKey() << std::endl;
	std::vector<unsigned char> request = Serializer::SerializeRSAKeyExchangeInitiation(rsa.GetPublicKey(), rsa.GetProduct());
	
	std::cout << '1';
	SendData(socket, request);

	std::cout << '1';
	std::vector<unsigned char> response = ReceiveWithTimeout(socket);

	std::cout << '1';
	auto deciphered = rsa.Decrypt(RSA::PlainToCipher(response));

	std::cout << '1';

	return Deserializer::DeserializeRSAKeyExchangeInitiation(deciphered);
}

void Communicator::RecieveRSAHandshake(sf::TcpSocket& socket, const AES& aes)
{
	auto recv = ReceiveWithTimeout(socket);

	RSA rsa(Deserializer::DeserializeReceivedRSAKeyExchange(recv));

	SendData(socket, Serializer::SerializeReceivedRSAKeyExchange(aes, rsa));
}

bool Communicator::IsDirectoryMessage(const std::vector<unsigned char>& message)
{
	if (message[0] == 'D' && message[1] == 'I' && message[2] == 'R')
		return true;
	return false;
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
			std::cout << "Error code: " << (int)status << std::endl;
			socket.setBlocking(true);
			throw std::runtime_error("Socket error");
		}
	}
}

bool Communicator::HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time)
{
	return std::chrono::steady_clock::now() - start_time > Communicator::timeout;
}

sf::IpAddress Communicator::StringToIP(const std::string& ipString)
{
	std::uint8_t parts[4] = { 0, 0, 0, 0 };
	std::stringstream ss(ipString);
	std::string part;

	for (int i = 0; i < 4 && std::getline(ss, part, '.'); ++i) {
		int number = std::stoi(part);
		if (number >= 0 && number <= 255) {
			parts[i] = static_cast<std::uint8_t>(number);
		}
		else {
			// Handle invalid number
			throw std::runtime_error("Invalid IP address: " + ipString);
		}
	}

	return sf::IpAddress(parts[0], parts[1], parts[2], parts[3]);
}
