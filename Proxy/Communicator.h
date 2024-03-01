#pragma once
#include "NetworkHandler.h"
#include <vector>
#include <queue>
#include <string>
#include <chrono>
#include <memory>
#include "SFML/Network.hpp"

class Communicator
{
public:
	explicit Communicator(const NetworkHandler& nwh);
	~Communicator();

	[[noreturn]] void RunServer();
	static std::vector<unsigned char> GetRelays(const Directory& dir, const LoadLevel& ll);
private:
	// Server socket
	void HandleClient(std::unique_ptr<sf::TcpSocket> clientSocket);

	// Message Sending
	void SendMessages();
	static sf::TcpSocket::Status SendData(sf::TcpSocket& socket, const std::vector<unsigned char>& data);
	static std::vector<unsigned char> ReceiveWithTimeout(sf::TcpSocket& socket);

	// Helper functions
	static bool HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time);

	// Members
	std::queue <std::vector<unsigned char>> _messageQueue;
	sf::TcpListener _serverSocket;
	NetworkHandler _nwhandler;

	// Constants
	static const std::chrono::seconds timeout; // Defined in .cpp file

	static const unsigned short server_port = 8202;
};
