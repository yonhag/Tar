#pragma once
#include <vector>

class JsonDeserializer
{
public:
	// Returns RSAKey and AESKey
	static void DeserializeDirectoryConnectionRequest(const std::vector<unsigned char>& data, int& RSAKey, int& AESKey);
};