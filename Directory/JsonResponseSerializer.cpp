#include "JsonSerializer.h"

using json = nlohmann::json;

void to_json(json& j, const DedicatedRelay& relay)
{
	j = json{
		{ "IP", relay.ip },
		{ "AESKey", relay.AESKey },
		{ "RSAKey", relay.RSAKey }
	};
}

Response JsonSerializer::SerializeGetRelaysResponse(const std::vector<DedicatedRelay>& relays)
{
	Response response;
	std::vector<unsigned char> buffer;
	json j;

	// Adding the relays
	for (int i = 1; i <= relays_per_user; i++)
		j["Relay" + DigitToChar(i)] = relays[i];

	// Turning the json object to a byte vector
	auto jsonString = j.dump();

	// Adding to the initial vector
	for (const auto& byte : jsonString)
		buffer.push_back(byte);

	// Returning
	response.data = buffer;

	return response;
}

std::vector<unsigned char> JsonSerializer::SerializeRelayConnectionRequest()
{
	std::vector<unsigned char> vec;

	const char* signature = "DIR1";
	vec.insert(vec.begin(), signature, signature + sizeof(signature) - 1);

	return vec;
}

char JsonSerializer::DigitToChar(const int integer)
{
	if (integer > 9 && integer < 0)
		throw std::exception("Must be a digit");
	return static_cast<char>(integer + '0');
}
