#include <WinSock2.h>

#define port 8200

class Communicator
{
public:
	Communicator();
	~Communicator();

private:
	SOCKET m_serverSocket;
	//functions
	void bindAndListen();

};
