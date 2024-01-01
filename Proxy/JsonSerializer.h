#include "LoadLevel.h"
#include <vector>

class JsonSerializer
{
public:
	static std::vector<unsigned char> SerializeGetRelaysRequest(const LoadLevel loadlevel);
};
