#pragma once
#include "json.hpp"
#include "Response.h"
#include "DedicatedRelay.h"

class JsonSerializer
{
public:
	static Response SerializeGetRelaysResponse(const std::vector<DedicatedRelay>& relays);
	static std::vector<unsigned char> SerializeRelayConnectionRequest();

private:
	static char DigitToChar(const int integer);
};
