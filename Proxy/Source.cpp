#include "Communicator.h"
#include "WSAInitializer.h"

int main()
{
	WSAInitializer wsa_init;
	auto comm = Communicator();
	if (!comm.ConnectToNetwork())
		// throw exception
		NULL;
	
	while (true)
	{
		comm.UseNetwork();
	}

	return 0;
}