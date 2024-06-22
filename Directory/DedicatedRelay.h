#pragma once
#include "Relay.h"
#include <AES.h>
#include <string>

struct DedicatedRelay
{
	std::string ip;
	unsigned short port;
	AES key;

public:
	bool operator==(const Relay& other) const
	{
		if (this->ip == other.ip)
			return true;
		return false;
	}
};

// Constant needed in every file this struct's included in
static const int relays_per_user = 3;
