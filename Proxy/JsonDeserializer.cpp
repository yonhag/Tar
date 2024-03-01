#include "JsonDeserializer.h"
#include "json.hpp"

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
