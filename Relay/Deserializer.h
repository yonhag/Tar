#pragma once
#include <vector>

class Deserializer
{
public:
	// Returns RSAKey and AESKey
	static void DeserializeDirectoryConnectionRequest(const std::vector<unsigned char>& data, int& RSAKey, int& AESKey);
	static bool DeserializeDirectoryConnectionResponse(const std::vector<unsigned char>& response);
};