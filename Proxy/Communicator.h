#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <queue>
#include <string>
#include "NetworkHandler.h"

class Communicator
{
public:
	explicit Communicator(const NetworkHandler& nwh);
	~Communicator();

	[[noreturn]] void RunServer();

private:
	// Server socket
	void bindAndListen();
	void HandleClient(SOCKET clientSocket);

	// Message Sending
	void SendMessages();

	std::queue <std::vector<unsigned char>> _messageQueue;
	SOCKET _serverSocket;
	NetworkHandler _nwhandler;

	const u_short port = 8200;
};
