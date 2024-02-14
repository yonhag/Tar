#include "Deserializer.h"
#include "json.hpp"

using json = nlohmann::json;

void Deserializer::DeserializeDirectoryConnectionRequest(const std::vector<unsigned char>& data, int& RSAKey, int& AESKey)
{
	json j;
	j = json::parse(data);

	RSAKey = j["RSAKey"];
	AESKey = j["AESKey"];
}

bool Deserializer::DeserializeDirectoryConnectionResponse(const std::vector<unsigned char>& response)
{
	if (response.size() != 2)
		return false;
	
	const std::string OKMessage = "OK";
	
	if (response[0] == OKMessage[0] && response[1] == OKMessage[1])
		return true;
	return false;
}
