#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include "NetworkHandler.h"

class Communicator
{
public:
	explicit Communicator(const NetworkHandler& nwh);
	~Communicator();

	[[noreturn]] void RunServer();

private:
	void bindAndListen();
	void HandleClient(SOCKET clientSocket);
	void SendData(SOCKET sock, const std::vector<unsigned char>& data);
	void UseNetwork();

	
	SOCKET _serverSocket;
	std::map<SOCKET, RequestHandler> _user_list;
	NetworkHandler _nwhandler;

	const u_short port = 8200;
};
