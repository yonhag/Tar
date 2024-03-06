#include "Relay.h"
#include "Response.h"
#include "Request.h"
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include "SFML/Network.hpp"

class Communicator
{
public:
	Communicator();
	~Communicator();

	[[noreturn]] void RunServer();
	static Response SendRelayConnectionRequest(const Relay& relay, const std::vector<unsigned char>& request);
	static unsigned int UpdateOtherDirectories(const Request& relayRequest);

private:
	void HandleClient(std::unique_ptr<sf::TcpSocket> clientSocket);

	// Helper functions
	static std::vector<unsigned char> SendDataThroughNewClientSocket(const std::string& ip, const unsigned short port, const std::vector<unsigned char>& data);
	static sf::TcpSocket::Status SendData(sf::TcpSocket& socket, const std::vector<unsigned char>& data);
	static std::vector<unsigned char> ReceiveWithTimeout(sf::TcpSocket& socket);
	static bool HasTimeoutPassed(const std::chrono::steady_clock::time_point& start_time);
	static sf::IpAddress StringToIP(const std::string& ipString);

	// Members
	sf::TcpListener _serverSocket;
	std::vector<std::unique_ptr<sf::TcpSocket>> _user_list;

	// Consts
	static const std::chrono::seconds timeout; // Can be changed in Communicator.cpp file
	static const unsigned short directory_listening_port = 8201;
	static const int max_message_size = 2048; // in bytes
};
