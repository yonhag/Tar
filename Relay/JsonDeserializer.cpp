#include "JsonDeserializer.h"
#include "json.hpp"

using json = nlohmann::json;

void JsonDeserializer::DeserializeDirectoryConnectionRequest(const std::vector<unsigned char>& data, int& RSAKey, int& AESKey)
{
	json j;
	j = json::parse(data);

	RSAKey = j["RSAKey"];
	AESKey = j["AESKey"];
}
