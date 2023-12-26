#include "json.hpp"
#include "Request.h"
#include "Response.h"
#include "DedicatedRelay.h"
#include "Relay.h"



class JsonDeserializer
{
public:
	static DedicatedRelay DeserializeRelayConnectionResponse(const Response& response);
	static bool DeserializeUpdateDirectoriesResponse(const std::vector<unsigned char>& response);
	static Relay DeserializeUpdateDirectoryRequest(const std::vector<unsigned char>& request);
};
