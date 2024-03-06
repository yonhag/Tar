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

std::pair<std::string, std::vector<unsigned char>> Deserializer::DeserializeClientMessages(const std::vector<unsigned char>& data)
{
	std::pair<std::string, std::vector<unsigned char>> desUserMessage;
	
	json j;
	j = json::parse(data);
	desUserMessage.first = j["DestIP"];
	desUserMessage.second = std::vector<unsigned char>(j["Data"]);

	return desUserMessage;
}
