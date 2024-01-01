#include "JsonSerializer.h"
#include "json.hpp"

using json = nlohmann::json;

std::vector<unsigned char> JsonSerializer::SerializeGetRelaysRequest(const LoadLevel loadlevel)
{
	json j;

	j["LoadLevel"] = loadlevel;

	auto jsonString = j.dump();

	std::vector<unsigned char> buffer;
	for (const auto& byte : jsonString)
		buffer.push_back(byte);

	return buffer;
}
