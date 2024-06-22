#pragma once
#include <vector>
#include <string>
#include <RSA.h>
#include <AES.h>
#include "Request.h"

class Deserializer
{
public:
	// Returns RSAKey and AESKey
	static void DeserializeDirectoryConnectionRequest(const std::vector<unsigned char>& data, int& RSAKey, int& AESKey);
	static bool DeserializeDirectoryConnectionResponse(const std::vector<unsigned char>& response);
	static Request DeserializeClientMessages(const std::vector<unsigned char>& data);
	static unsigned int DeserializeServeRequest(const std::vector<unsigned char>& data);
	
	static RSA DeserializeReceivedRSAKeyExchange(std::vector<unsigned char>& message);
	static AES DeserializeRSAKeyExchangeInitiation(std::vector<unsigned char>& message);
};