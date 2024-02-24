#include "Communicator.h"

int main(int argc, char** argv)
{
	if (argc == 1)
		std::cout << "Invalid argc" << std::endl;

	try
	{
		Communicator(static_cast<unsigned short>(std::stoul(std::string(argv[0])))).RunServer();
	}
	catch (std::exception& e) { std::cout << e.what(); }
}