#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <vector>
#include <map>
#include <string>
#include "RequestHandler.h"

class Communicator
{
public:
	Communicator();
	~Communicator();

	[[noreturn]] void RunServer();

private:
	void HandleConnection(SOCKET clientSocket);
	void ServeClient(SOCKET incomingSocket, const Request& initialRequest);
	std::vector<unsigned char> ReceiveSocketMessageAsVector(SOCKET targetSocket);

	
	
		
	SOCKET _serverSocket;
	std::map<SOCKET, RequestHandler> _user_list;
	
	const DWORD socket_timeout = 10000; // 10s
	const u_short port = 8200;

	// Helper functions
	void BindAndListen();
	void SendData(SOCKET sock, const std::vector<unsigned char>& data);
	bool IsDirectoryMessage(const std::vector<unsigned char>& message);
};
