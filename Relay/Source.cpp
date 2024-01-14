#pragma comment (lib, "ws2_32.lib")
#include "Communicator.h"
#include "WSAInitializer.h"
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
	std::cout << "Hello World" << std::endl;
	std::this_thread::sleep_for(std::chrono::minutes(2));
	WSAInitializer wsa_init;
	Communicator().RunServer();

	return 0;
}