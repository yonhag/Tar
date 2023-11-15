#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <map>
#include <string>

class Communicator
{
public:
	Communicator();
	~Communicator();

	[[noreturn]] void RunServer();

private:
	void bindAndListen();
	void HandleClient(SOCKET clientSocket);
	void SendData(SOCKET sock, const std::vector<unsigned char>& data);

	SOCKET _serverSocket;
	std::map<std::string, unsigned int> _ip_list; // IP, Key

	const u_short port = 8200;
};
