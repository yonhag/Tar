#pragma once
#include "json.hpp"
#include "Response.h"
#include "DedicatedRelay.h"

class JsonResponseSerializer
{
public:
	static Response SerializeGetRelaysResponse(const std::vector<DedicatedRelay>& relays);

private:
	static char DigitToChar(const int integer);
};
