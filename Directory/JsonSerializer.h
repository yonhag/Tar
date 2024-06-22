#pragma once
#include "DedicatedRelay.h" // Relay included here
#include "Response.h"
#include "Request.h"
#include "AES.h"
#include "RSA.h"
#include "json.hpp"

class JsonSerializer
{
public:
	static Response SerializeGetRelaysResponse(const std::vector<DedicatedRelay>& relays, const unsigned int sessionID);
	static Request SerializeUpdateDirectoryRequest(const Relay& newRelay);
	static std::vector<unsigned char> SerializeRelayConnectionRequest();
	static std::vector<unsigned char> SerializeRSAKeyExchange(const AES& aes, const RSA& rsa);
};
