#include "JsonSerializer.h"
#include "DirectoryCodes.h"

using json = nlohmann::json;



void to_json(json& j, const DedicatedRelay& relay)
{
	j = json{
		{ "IP", relay.ip },
		{ "Port", relay.port }
	};
}

void to_json(json& j, const Relay& relay)
{
	j = json{
		{"IP", relay.ip},
		{"Bandwidth", relay.bandwidth}
	};
}

Response JsonSerializer::SerializeGetRelaysResponse(const std::vector<DedicatedRelay>& relays)
{
	Response response;
	std::vector<unsigned char> buffer;
	json j;

	// Adding the relays
	for (int i = 0; i < relays_per_user; i++)
		j["Relay" + std::to_string(i + 1)] = relays[i];

	// Turning the json object to a byte vector
	auto jsonString = j.dump();

	// Adding to the initial vector
	for (const auto& byte : jsonString)
		buffer.push_back(byte);

	// Returning
	response.data = buffer;

	return response;
}

Request JsonSerializer::SerializeUpdateDirectoryRequest(const Relay& newRelay)
{
	Request request;
	std::vector<unsigned char> buffer;

	// Adding signature
	std::string sigStr = "DIR" + std::to_string(static_cast<int>(DirectoryCodes::AddRelay));
	const char* signature = sigStr.c_str();
	buffer.insert(buffer.begin(), signature, signature + sizeof(signature) - 1);
	
	// Creating JSON object
	json j;
	j["Relay"] = newRelay;

	// Turning the json object to a byte vector
	auto jsonString = j.dump();

	// Adding to the initial vector
	for (const auto& byte : jsonString)
		buffer.push_back(byte);

	request.data = buffer;
	return request;
}

std::vector<unsigned char> JsonSerializer::SerializeRelayConnectionRequest()
{
	std::vector<unsigned char> vec;
	
	std::string sigStr = "DIR" + std::to_string(static_cast<int>(DirectoryCodes::RelayConnection));
	const char* signature = sigStr.c_str();
	vec.insert(vec.begin(), signature, signature + sizeof(signature) - 1);

	return vec;
}
