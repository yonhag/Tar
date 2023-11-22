#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include "cryptopp/rsa.h"

class Communicator
{
public:
	Communicator();
	~Communicator();

	[[noreturn]] void RunServer();

private:
	void BindAndListen();
	void HandleClient(SOCKET clientSocket);
	bool IsDirectoryMessage(const std::vector<unsigned char>& message);


	void SendData(SOCKET sock, const std::vector<unsigned char>& data);

	SOCKET _serverSocket;
	std::vector<SOCKET> _user_list;

	const u_short port = 8200;
};
