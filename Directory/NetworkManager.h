#pragma once
#include "Relay.h"
#include "DedicatedRelay.h"
#include "LoadLevel.h"
#include <vector>

enum class LevelBandwidths { Low = 10, Medium = 100, High = 1000 };

class NetworkManager
{
public:
	static std::vector<DedicatedRelay> GetRelays(const LoadLevel loadlevel);
	static void JoinNetwork(const std::string& ip, const unsigned int bandwidth);
	static bool AddRelay(const Relay& relay);
	static bool RemoveRelay(const Relay& relay);

private:
	static DedicatedRelay DedicateRelay(const Relay& relay);
	static std::vector<DedicatedRelay> DedicateRelaysForNormalLoadUser();

	// Sorted by bandwidth ascending - Lower bandwidth -> Higher banwidth
	static std::vector<Relay> _relays;
};
