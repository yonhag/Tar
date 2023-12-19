#pragma once
#include <string>

struct DedicatedRelay
{
	std::string ip;
	int AESKey;
	int RSAKey;
};

// Constant needed in every file this struct's included in
static const int relays_per_user = 3;
