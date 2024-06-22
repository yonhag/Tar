#pragma once
#include "DedicatedRelay.h"
#include <AES.h>
#include <vector>

struct Session 
{
	std::vector<DedicatedRelay> _relays;
	std::vector<AES> _keys;
};