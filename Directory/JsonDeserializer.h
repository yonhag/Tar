#include "json.hpp"
#include "Request.h"
#include "Response.h"
#include "DedicatedRelay.h" // Relay included here
#include "LoadLevel.h"
#include "RSA.h"

class JsonDeserializer
{
public:
	static LoadLevel DeserializeGetRelaysRequest(const std::vector<unsigned char>& request);

	static Relay DeserializeRelayConnectionRequest(const std::vector<unsigned char>& request);
	
	static AES DeserializeRelayDedicationResponse(const Response& response);
	
	static bool DeserializeUpdateDirectoriesResponse(const std::vector<unsigned char>& response);
	
	static Relay DeserializeUpdateDirectoryRequest(const std::vector<unsigned char>& request);

	static RSA DeserializeRSAKeyExchange(const std::vector<unsigned char>& message);
};
