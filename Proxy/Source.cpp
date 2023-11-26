#include "Communicator.h"
#include "NetworkHandler.h"
#include "WSAInitializer.h"

int main()
{
	WSAInitializer wsa_init;
	NetworkHandler nwh;

	if (!nwh.isConnected())
		return 1;

	auto comm = Communicator(nwh);

	while (true)
	{
		comm.UseNetwork();
	}

	return 0;
}