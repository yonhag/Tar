#include "Serializer.h"
#include "json.hpp"

using json = nlohmann::json;

std::vector<unsigned char> Serializer::SerializeDirectoryConnectionResponse(const int AESKey, const int RSAKey)
{
	std::vector<unsigned char> response;

	json j;
	j["RSAKey"] = RSAKey;
	j["AESKey"] = AESKey;

	auto dump = j.dump();
	for (const auto& byte : dump)
		response.push_back(byte);

	return response;
}

std::vector<unsigned char> Serializer::SerializeDirectoryConnectionRequest(const std::string& ip, const unsigned int BandwidthInMb, const unsigned short listeningPort)
{
	std::vector<unsigned char> request;

	json j;
	j["IP"] = ip; 
	j["Bandwidth"] = BandwidthInMb;
	j["Port"] = listeningPort;

	auto dump = j.dump();

	request.push_back('2'); // Adding signature

	for (auto& byte : dump)
		request.push_back(byte);

	return request;
}
