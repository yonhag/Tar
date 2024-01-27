#include "NetworkHandler.h"
#include <vector>
#include <queue>
#include <string>
#include <chrono>
#include <memory>
#include "SFML/Network.hpp"
#include "SFML/System.hpp"

class Communicator
{
public:
	explicit Communicator(const NetworkHandler& nwh);
	~Communicator();

	[[noreturn]] void RunServer();

private:
	// Server socket
	void HandleClient(std::unique_ptr<sf::TcpSocket> clientSocket);

	// Message Sending
	void SendMessages();
	sf::TcpSocket::Status SendData(sf::TcpSocket& socket, const std::vector<unsigned char>& data) const;
	static std::vector<unsigned char> ReceiveWithTimeout(sf::TcpSocket& socket);

	// Helper functions
	static bool HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time);

	std::queue <std::vector<unsigned char>> _messageQueue;
	sf::TcpListener _serverSocket;
	NetworkHandler _nwhandler;

	const u_short server_port = 8200;
	const u_short relay_port = 8200;
};
