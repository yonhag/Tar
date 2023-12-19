#pragma once
#include "Relay.h"
#include <vector>

class NetworkHandler
{
public:
	static std::vector<Relay> GetRelays();
	static bool JoinNetwork();

private:
	static std::vector<Relay> _relays;
	static const int relays_per_user = 3;
};