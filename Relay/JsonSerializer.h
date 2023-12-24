#pragma once
#include <vector>

class JsonSerializer
{
public:
	static std::vector<unsigned char> SerializeDirectoryConnectionResponse(const int AESKey, const int RSAKey);
};