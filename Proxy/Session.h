#pragma once
#include "Relay.h"
#include <vector>

struct Session
{
	std::vector<Relay> _relays;
	unsigned int _id;
};