#include "Relay.h"
#include "Response.h"
#include "Request.h"
#include <string>
#include <memory>
#include <vector>
#include <map>
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
	void BindAndListen();
	void HandleClient(std::unique_ptr<sf::TcpSocket> clientSocket);

	// Helper functions
	sf::TcpSocket::Status SendData(sf::TcpSocket& socket, const std::vector<unsigned char>& data) const;
	std::vector<unsigned char> ReceiveWithTimeout(sf::TcpSocket& socket);

	sf::TcpListener _serverSocket;
	std::vector<std::unique_ptr<sf::TcpSocket>> _user_list;

	// Consts
	static const unsigned short network_listening_port = 8200;
	static const int max_message_size = 2048; // in bytes
};
