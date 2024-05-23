#include "Communicator.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Invalid argc" << std::endl;
		return 1;
	}
	try
	{
		std::cout << argv[1] << std::endl;
		Communicator(static_cast<unsigned short>(std::stoul(std::string(argv[1])))).RunServer();
	}
	catch (std::exception& e) { std::cout << "Exception: " << e.what(); }
}