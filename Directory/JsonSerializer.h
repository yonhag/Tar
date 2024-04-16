#pragma once
#include "json.hpp"
#include "Response.h"
#include "Request.h"
#include "DedicatedRelay.h" // Relay included here
#include "AES.h"
#include "RSA.h"

class JsonSerializer
{
public:
	static Response SerializeGetRelaysResponse(const std::vector<DedicatedRelay>& relays);
	static Request SerializeUpdateDirectoryRequest(const Relay& newRelay);
	static std::vector<unsigned char> SerializeRelayConnectionRequest();
	static std::vector<unsigned char> SerializeRSAKeyExchange(const AES& aes, const RSA& rsa);
};
