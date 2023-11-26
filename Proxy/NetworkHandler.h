#pragma once
#include "Relay.h"
#include <vector>
#include <string>

class NetworkHandler
{
public:
	NetworkHandler();
	bool isConnected() const;


private:
	bool _isConnected;
	std::vector<Relay> _relays;
};