#pragma once
#include <vector>
#include <string>
#include <RSA.h>
#include <AES.h>

class Deserializer
{
public:
	// Returns RSAKey and AESKey
	static void DeserializeDirectoryConnectionRequest(const std::vector<unsigned char>& data, int& RSAKey, int& AESKey);
	static bool DeserializeDirectoryConnectionResponse(const std::vector<unsigned char>& response);
	static std::pair<std::string, std::vector<unsigned char>> DeserializeClientMessages(const std::vector<unsigned char>& data);
	
	static RSA DeserializeReceivedRSAKeyExchange(std::vector<unsigned char>& message);
	static AES DeserializeRSAKeyExchangeInitiation(std::vector<unsigned char>& message);
};