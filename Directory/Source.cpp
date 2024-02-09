#include "Communicator.h"
#include <exception>
#include <iostream>

int main()
{
	try
	{
		Communicator().RunServer();
	}
	catch (std::exception& e) { std::cout << e.what(); }
	return 0;
}