#include "json.hpp"
#include "Request.h"

class JsonRequestDeserializer
{
public:
	static Request DeserializeGetRelaysRequests(const std::vector<unsigned char>& request);
};