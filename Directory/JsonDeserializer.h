#include "json.hpp"
#include "Request.h"
#include "Response.h"
#include "DedicatedRelay.h"
#include "Relay.h"
#include "LoadLevel.h"

class JsonDeserializer
{
public:
	static LoadLevel DeserializeGetRelaysRequest(const std::vector<unsigned char>& request);
	static DedicatedRelay DeserializeRelayConnectionResponse(const Response& response);
	static bool DeserializeUpdateDirectoriesResponse(const std::vector<unsigned char>& response);
	static Relay DeserializeUpdateDirectoryRequest(const std::vector<unsigned char>& request);
};
