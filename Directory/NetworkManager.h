#pragma once
#include "Relay.h"
#include "DedicatedRelay.h"
#include <vector>

class NetworkManager
{
public:
	static std::vector<DedicatedRelay> GetRelays();
	static bool JoinNetwork();

private:
	static DedicatedRelay DedicateRelay(const Relay& relay);

	static std::vector<Relay> _relays;
};