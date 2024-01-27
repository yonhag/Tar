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
	static std::vector<unsigned char> GetRelays(const std::string& dirIP);

private:
	// Server socket
	void HandleClient(std::unique_ptr<sf::TcpSocket> clientSocket);

	// Message Sending
	void SendMessages();
	sf::TcpSocket::Status SendData(sf::TcpSocket& socket, const std::vector<unsigned char>& data) const;
	static std::vector<unsigned char> ReceiveWithTimeout(sf::TcpSocket& socket);

	// Helper functions
	static bool HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time);

	// Members
	std::queue <std::vector<unsigned char>> _messageQueue;
	sf::TcpListener _serverSocket;
	NetworkHandler _nwhandler;

	// Constants
	static const std::chrono::seconds timeout; // Defined in .cpp file

	static const unsigned short relay_port = 8200;
	static const unsigned short directory_port = 8201;
	static const unsigned short server_port = 8202;
};
