#include "Communicator.h"

int main()
{
	try
	{
		Communicator().RunServer();
	}
	catch (std::exception& e) { std::cout << e.what(); }
}