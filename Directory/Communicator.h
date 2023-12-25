#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include "Relay.h"
#include "Response.h"

class Communicator
{
public:
	Communicator();
	~Communicator();

	[[noreturn]] void RunServer();
	static Response SendRelayConnectionRequest(const Relay& relay, const std::vector<unsigned char>& request);
	static int UpdateOtherDirectories(const Relay& newRelay);


private:
	void BindAndListen();
	void HandleClient(SOCKET clientSocket);

	void SendData(SOCKET sock, const std::vector<unsigned char>& data);
	static PCWSTR StringToPCWSTR(const std::string& str);

	SOCKET _serverSocket;
	std::vector<SOCKET> _user_list;

	// Consts
	static const u_short relay_port = 8200;
	static const int max_message_size = 2048; // in bytes
};
