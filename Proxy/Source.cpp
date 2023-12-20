#include "Communicator.h"
#include "NetworkHandler.h"
#include "WSAInitializer.h"

int main()
{
	WSAInitializer wsa_init;
	NetworkHandler nwh;

	if (!nwh.IsConnected())
		return 1;

	auto comm = Communicator(nwh);

	while (true)
		comm.RunServer();

	return 0;
}