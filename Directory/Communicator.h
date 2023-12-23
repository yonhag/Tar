#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include "Relay.h"

class Communicator
{
public:
	Communicator();
	~Communicator();

	[[noreturn]] void RunServer();
	static Response SendRelayConnectionRequest(const Relay& relay, const std::vector<unsigned char>& request);

private:
	void BindAndListen();
	void HandleClient(SOCKET clientSocket);


	void SendData(SOCKET sock, const std::vector<unsigned char>& data);
	static PCWSTR StringToPCWSTR(const std::string& str);

	SOCKET _serverSocket;
	std::vector<SOCKET> _user_list;

	static const u_short relay_port = 8200;
	static const int max_message_size = 20480; // in bytes
};
