#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include "NetworkHandler.h"

class Communicator
{
public:
	Communicator(const NetworkHandler& nwh);
	~Communicator();
	
	void UseNetwork();

private:
	NetworkHandler _nwhandler;
};