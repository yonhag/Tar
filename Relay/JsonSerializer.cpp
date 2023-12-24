#include "JsonSerializer.h"
#include "json.hpp"

using json = nlohmann::json;

std::vector<unsigned char> JsonSerializer::SerializeDirectoryConnectionResponse(const int AESKey, const int RSAKey)
{
	std::vector<unsigned char> response(0);

	json j;
	j["RSAKey"] = RSAKey;
	j["AESKey"] = AESKey;

	auto dump = j.dump();
	for (const auto& byte : dump)
		response.push_back(byte);

	return response;
}
