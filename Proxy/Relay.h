#pragma once
#include <string>

struct Relay
{
	unsigned long _publicRSAKey;
	unsigned long _publicAESKey	;
	std::string _ip;
};