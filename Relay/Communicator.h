#include <WinSock2.h>
#include <vector>

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
	std::vector<std::string> _ip_list;

	const u_short port = 8200;
};
