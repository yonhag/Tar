#pragma once
#include "Relay.h"
#include <vector>

class NetworkHandler
{
public:
	std::vector<Relay> GetRelays();
	bool JoinNetwork();


private:
	std::vector<Relay> _relays;
	static const int relays_per_user = 3;
};