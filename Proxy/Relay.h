#pragma once
#include <string>

struct Relay
{
	std::string _ip;
	unsigned short _port;
	unsigned long _publicRSAKey;
	unsigned long _publicAESKey	;
};