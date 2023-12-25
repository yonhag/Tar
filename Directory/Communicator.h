#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include "Relay.h"
#include "Response.h"
#include "Request.h"

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
	void HandleClient(SOCKET clientSocket);

	// Helper functions
	static std::vector<unsigned char> SendDataThroughNewClientSocket(const std::string& ip, const u_short port, const std::vector<unsigned char>& data);
	static void SendData(SOCKET sock, const std::vector<unsigned char>& data);
	static PCWSTR StringToPCWSTR(const std::string& str);

	SOCKET _serverSocket;
	std::vector<SOCKET> _user_list;

	// Consts
	static const u_short network_listening_port = 8200;
	static const int max_message_size = 2048; // in bytes
};
