#pragma once
#include <string>

struct Relay
{
	unsigned int _publicRSAKey;
	unsigned int _publicAESKey;
	std::string _ip;
};