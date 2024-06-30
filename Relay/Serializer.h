#pragma once
#include "Request.h"
#include <AES.h>
#include <RSA.h>
#include <vector>
#include <string>

class Serializer
{
public:
	static std::vector<unsigned char> SerializeAES(const AES& aes);
	static std::vector<unsigned char> SerializeDirectoryConnectionRequest(const std::string& ip, const unsigned int BandwidthInMb, const unsigned short listeningPort);
	
	static std::vector<unsigned char> SerializeRSAKeyExchangeInitiation(const PublicKey& key, const Product& prod);
};