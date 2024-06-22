#pragma once
#include "DedicatedRelay.h"
#include <AES.h>
#include <vector>

struct Session 
{
	std::vector<DedicatedRelay> _relays;
	unsigned int _id;
};