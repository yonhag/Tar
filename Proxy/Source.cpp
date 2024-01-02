#include "Communicator.h"
#include "NetworkHandler.h"
#include "WSAInitializer.h"
#include "LoadLevel.h"
#include <iostream>

int main()
{
	WSAInitializer wsa_init;

	LoadLevel loadlevel;
	int lvl;
	std::cout << "Enter load level:\n0: Low\n1: Medium\n2: High\nType: ";
	std::cin >> lvl;
	if (lvl < 0 || lvl > 2)
		loadlevel = LoadLevel::High;
	else
		loadlevel = (LoadLevel)lvl;
	NetworkHandler nwh(loadlevel);

	if (!nwh.IsConnected())
		return 1;

	auto comm = Communicator(nwh);

	while (true)
		comm.RunServer();

	return 0;
}