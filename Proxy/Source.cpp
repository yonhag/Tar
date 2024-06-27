#include "Communicator.h"
#include "NetworkHandler.h"
#include "LoadLevel.h"
#include <exception>
#include <iostream>

int main()
{
	LoadLevel loadlevel = LoadLevel::Low;
	/*
	int lvl;
	std::cout << "Enter load level:\n0: Low\n1: Medium\n2: High\nType: ";
	std::cin >> lvl;
	if (lvl < 0 || lvl > 2)
		loadlevel = LoadLevel::High;
	else
		loadlevel = (LoadLevel)lvl;
	*/
	try
	{
		NetworkHandler nwh(loadlevel);

		if (!nwh.IsConnected())
		{
			std::cout << "Directories unavailable" << std::endl;
			return 1;
		}

		auto comm = Communicator(nwh);

		while (true)
			comm.RunServer();
	}
	catch (std::exception& e) { std::cout << e.what(); }

	return 0;
}
