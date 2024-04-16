#include "Communicator.h"
#include "Protocol.h"
#include "Consts.h"
#include "JsonDeserializer.h"
#include "JsonSerializer.h"
#include <sstream>
#include <thread>
#include <exception>
#include <iostream>

const std::chrono::seconds Communicator::timeout = std::chrono::seconds(5);

Communicator::Communicator(const NetworkHandler& nwh) :
	_nwhandler(nwh)
{
	this->_serverSocket.listen(Communicator::server_port);
}

Communicator::~Communicator()
{
	this->_serverSocket.close();
}

void Communicator::RunServer()
{
	std::cout << "Listening on port " << Communicator::server_port << std::endl;

	std::thread sendingThread(&Communicator::SendMessages, this);
	sendingThread.detach();

	while (true)
	{
		// Accepting clients
		auto clientSocket = std::make_unique<sf::TcpSocket>();
		if (this->_serverSocket.accept(*clientSocket) == sf::Socket::Status::Done)
		{
			std::cout << "Accepting client..." << std::endl;

			// Detaching client handling to a different thread
			this->HandleClient(std::move(clientSocket));
		}
		else
			std::cout << "Problems with client connection" << std::endl;
	}

}

std::vector<unsigned char> Communicator::GetRelays(const Directory& dir, const LoadLevel& ll)
{
	std::vector<unsigned char> request = JsonSerializer::SerializeGetRelaysRequest(ll);

	sf::TcpSocket directorySocket;
	directorySocket.connect(dir._ip, dir._port);
	
	AES aes(Communicator::RSAKeyExchange(directorySocket));
	
	SendData(directorySocket, request);
	
	// Receiving the relays
	// #TODO: change the vector size to something more normal
	std::vector<unsigned char> relays(256);
	std::size_t recv;
	directorySocket.receive(relays.data(), relays.size(), recv); //ReceiveWithTimeout(directorySocket);

	return relays;
}

void Communicator::HandleClient(std::unique_ptr<sf::TcpSocket> sock)
{
	// Recieving the message
	std::vector<unsigned char> message = ReceiveWithTimeout(*sock);
	
	for (auto& i : message)
		std::cout << i;
	std::cout << std::endl;

	MessageRequest request = GetMessageRequest(message);

	std::cout << "Data: ";
	for (auto& i : request._data)
		std::cout << i;
	std::cout << std::endl << " IP: " << request._destIP;

	//std::vector<unsigned char> encrypted = this->_nwhandler.EncryptMessage(request);
	std::vector<unsigned char> encrypted = JsonSerializer::SerializeRelayDataSendingRequest(request);

	this->_messageQueue.push(encrypted);
}

void Communicator::SendMessages()
{
	sf::TcpSocket relaySocket;

	std::cout << "Connecting to " << this->_nwhandler.GetFirstRelayIP() << ':' << this->_nwhandler.GetFirstRelayPort() << std::endl;
	if (relaySocket.connect(this->_nwhandler.GetFirstRelayIP(), this->_nwhandler.GetFirstRelayPort()) != sf::Socket::Status::Done)
	{
		relaySocket.disconnect();
		std::cout << "Relay socket creation failed" << std::endl;
		return;
	}

	std::cout << "Success connecting!" << std::endl;

	while (true)
	{
		// Sleep if queue is empty
		while (this->_messageQueue.empty())
			std::this_thread::sleep_for(std::chrono::seconds(3));

		// Getting the message
		std::vector<unsigned char> message = this->_messageQueue.front();
		this->_messageQueue.pop();
		
		// std::vector<unsigned char> EncryptedMessage = Serialize

		// Sending the message
		if (SendData(relaySocket, message) != sf::Socket::Status::Done)
		{
			std::cout << "Error while sending message to client";
			return;
		}
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

AES Communicator::RSAKeyExchange(sf::TcpSocket& directorySocket)
{
	RSA rsa;

	std::vector<unsigned char> request = JsonSerializer::SerializeRSAHandshake(rsa.GetPublicKey(), rsa.GetProduct());

	SendData(directorySocket, request);

	std::vector<unsigned char> response = ReceiveWithTimeout(directorySocket);
	return JsonDeserializer::DesierlizeRSAHandshake(rsa.Decrypt(RSA::PlainToCipher(response)));
}

bool Communicator::HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time)
{
	return std::chrono::steady_clock::now() - start_time > Communicator::timeout;
}

MessageRequest Communicator::GetMessageRequest(const std::vector<unsigned char>& httpRequest)
{
	MessageRequest request;

	request._data = httpRequest;

	// Getting the host from the HTTP Request
	std::string host = GetHostFromRequest(std::string(httpRequest.begin(), httpRequest.end()));
	// Checking the IP of the host
	request._destIP = sf::IpAddress(host).toString();

	return request;
}

std::string Communicator::GetHostFromRequest(const std::string& httpRequest) 
{
	const std::string hostPrefix = "Host: ";

	std::istringstream requestStream(httpRequest);
	std::string line;

	// Iterate through each line of the request
	while (std::getline(requestStream, line)) {
		// Find the Host header
		if (line.substr(0, hostPrefix.size()) == hostPrefix) {
			// Extract the host value
			return TrimEndOfHost(line.substr(hostPrefix.size()));
		}
	}

	throw std::exception("No host found");
}

/*
* Removes the end of the string, if it is either '\n', '\r', or both of them.
* Returns the fixed string.
*/
std::string Communicator::TrimEndOfHost(const std::string& host) 
{
	size_t end = host.find_last_not_of("\r\n");
	return (end == std::string::npos) ? "" : host.substr(0, end + 1);
}