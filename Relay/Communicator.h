#include <vector>
#include <string>
#include "RequestHandler.h"
#include "SFML/Network.hpp"
#include <chrono>

class Communicator
{
public:
	Communicator();
	~Communicator();

	[[noreturn]] void RunServer();

private:
	void HandleConnection(std::unique_ptr<sf::TcpSocket> socket);
	void ServeClient(sf::TcpSocket& incomingSocket, const Request& initialRequest);
		
	sf::TcpListener _serverSocket;
	
	const unsigned short port = 8200;
	const std::chrono::seconds timeout = std::chrono::seconds(5);

	// Helper functions
	sf::TcpSocket::Status SendData(sf::TcpSocket& socket, const std::vector<unsigned char>& data) const;
	std::vector<unsigned char> ReceiveWithTimeout(sf::TcpSocket& socket);
	static bool IsDirectoryMessage(const std::vector<unsigned char>& message);
	bool HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time); // Uses class timout
};
