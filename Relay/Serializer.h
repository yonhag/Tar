#pragma once
#include <vector>
#include <string>
#include "Request.h"
#include <AES.h>
#include <RSA.h>

class Serializer
{
public:
	static DirResponse SerializeDirectoryServeResponse();
	static std::vector<unsigned char> SerializeDirectoryConnectionResponse(const int AESKey, const int RSAKey);
	static std::vector<unsigned char> SerializeDirectoryConnectionRequest(const std::string& ip, const unsigned int BandwidthInMb, const unsigned short listeningPort);
	
	static std::vector<unsigned char> SerializeReceivedRSAKeyExchange(const AES& aes, const RSA& rsa);
	static std::vector<unsigned char> SerializeRSAKeyExchangeInitiation(const PublicKey& key, const Product& prod);
};