#include "json.hpp"
#include "LoadLevel.h"

class JsonSerializer
{
public:
	static std::vector<unsigned char> SerializeGetRelaysRequest(const LoadLevel loadlevel);
};