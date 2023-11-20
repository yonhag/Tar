#pragma once
#include <WinSock2.h>
#include <Windows.h>

class Communicator
{
public:
	Communicator();
	~Communicator();

	bool ConnectToNetwork();
	
	void UseNetwork();

private:

};