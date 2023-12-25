#pragma once
#include "json.hpp"
#include "Response.h"
#include "Request.h"
#include "DedicatedRelay.h"
#include "Relay.h"

class JsonSerializer
{
public:
	static Response SerializeGetRelaysResponse(const std::vector<DedicatedRelay>& relays);
	static Request SerializeUpdateDirectoryRequest(const Relay& newRelay);
	static std::vector<unsigned char> SerializeRelayConnectionRequest();

private:
	static char DigitToChar(const int integer);
};
