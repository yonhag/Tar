#include "json.hpp"
#include "Request.h"
#include "Response.h"
#include "DedicatedRelay.h"

class JsonDeserializer
{
public:
	static DedicatedRelay DeserializeRelayConnectionResponse(const Response& response);
	static bool DeserializeUpdateDirectoriesResponse(const std::vector<unsigned char>& response);
};
