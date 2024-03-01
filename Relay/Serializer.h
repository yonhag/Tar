#pragma once
#include <vector>
#include <string>
#include "Request.h"

class Serializer
{
public:
	static DirResponse SerializeDirectoryServeResponse();
	static std::vector<unsigned char> SerializeDirectoryConnectionResponse(const int AESKey, const int RSAKey);
	static std::vector<unsigned char> SerializeDirectoryConnectionRequest(const std::string& ip, const unsigned int BandwidthInMb, const unsigned short listeningPort);

};