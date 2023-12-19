#include "JsonDecoder.h"
#include "json.hpp"
#include <codecvt>

using json = nlohmann::json;

void from_json(const json& j, Relay& relay)
{
	relay._ip = j["IP"];
	relay._publicAESKey = j["AESKey"];
	relay._publicRSAKey = j["RSAKey"];
}

MessageRequest JsonDecoder::DecodeClientMessage(const std::vector<unsigned char>& message)
{
	MessageRequest request;

	json j = json::parse(message);
	request._destIP = j["DestIP"];

	std::string data = j["Data"];
	request._data = std::vector<unsigned char>(data.begin(), data.end());

	return request;
}

std::vector<Relay> JsonDecoder::DecodeGetRelaysResponse(const std::vector<unsigned char>& response)
{
	std::vector<Relay> relays;
	
	json j = json::parse(response);
	
	for (int i = 1; i <= 3; i++)
		relays.push_back(j["Relay" + std::to_string(i)]);

	return relays;
}
