#pragma once
#include "Relay.h"
#include "DedicatedRelay.h"
#include <vector>

class NetworkManager
{
public:
	static std::vector<DedicatedRelay> GetRelays();
	// TODO: Add bandwidth support, to Relay.h too
	static void JoinNetwork(const std::string& ip, const unsigned int bandwidth);

private:
	static DedicatedRelay DedicateRelay(const Relay& relay);

	static std::vector<Relay> _relays;
};