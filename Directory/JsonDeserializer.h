#include "json.hpp"
#include "Request.h"
#include "Response.h"
#include "DedicatedRelay.h"
#include "LoadLevel.h"

class JsonDeserializer
{
public:
	static LoadLevel DeserializeGetRelaysRequest(const std::vector<unsigned char>& request);
	static DedicatedRelay DeserializeRelayConnectionResponse(const Response& response);
};
