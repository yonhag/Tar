#include "Directory.h"
#include "RequestHandler.h"
#include <vector>
#include <chrono>
#include "SFML/Network.hpp"

class Communicator
{
public:
	explicit Communicator(unsigned short port);
	~Communicator();

	[[noreturn]] void RunServer();

private:
	void HandleConnection(std::unique_ptr<sf::TcpSocket> socket);
	void ServeClient(sf::TcpSocket& incomingSocket, const Request& initialRequest);
	bool ConnectToDirectory(const Directory& dir);

	sf::TcpListener _serverSocket;
	
	unsigned short _listening_port;
	const unsigned short client_port = 8202;
	static const std::chrono::seconds timeout; // Defined in .cpp file

	// Helper functions
	sf::TcpSocket::Status SendData(sf::TcpSocket& socket, const std::vector<unsigned char>& data) const;
	static std::vector<unsigned char> ReceiveWithTimeout(sf::TcpSocket& socket);
	static bool IsDirectoryMessage(const std::vector<unsigned char>& message);
	static bool HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time);
	static sf::IpAddress StringToIP(const std::string& ipString);
};
