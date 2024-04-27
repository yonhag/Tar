#include "JsonDeserializer.h"

using json = nlohmann::json;

void from_json(const json& j, Relay& relay)
{
	relay._ip = j["IP"];
	relay._port = j["Port"];
}

std::vector<Relay> JsonDeserializer::DeserializeGetRelaysResponse(const std::vector<unsigned char>& response)
{
	std::vector<Relay> relays;
	
	json j = json::parse(response);
	
	for (int i = 1; i <= 3; i++)
		relays.push_back(j["Relay" + std::to_string(i)]);

	return relays;
}

AES JsonDeserializer::DeserializeRSAHandshake(const std::vector<unsigned char>& DecipheredResponse)
{
	json j = json::parse(DecipheredResponse);
	auto key = JsonToUCArray(j["AESKey"]);
	auto iv = JsonToUCArray(j["AESIV"]);
	return AES(key.get(), iv.get(), AESKeyLength::AES_128);
}

std::unique_ptr<unsigned char[]> JsonDeserializer::JsonToUCArray(const json& jsonArray)
{
	size_t size = jsonArray.size();
	std::unique_ptr<unsigned char[]> result(new unsigned char[size]);

	for (size_t i = 0; i < size; ++i)
		result[i] = jsonArray[i];

	return result;
}
