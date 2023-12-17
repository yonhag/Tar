#pragma comment (lib, "ws2_32.lib")
#include "Communicator.h"
#include "WSAInitializer.h"

int main()
{
	WSAInitializer wsa_init;
	Communicator().RunServer();

	return 0;
}