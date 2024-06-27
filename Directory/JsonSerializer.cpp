#include "JsonSerializer.h"
#include "DirectoryCodes.h"

using json = nlohmann::json;



void to_json(json& j, const DedicatedRelay& relay)
{
	j = json {
		{ "IP", relay.ip },
		{ "Port", relay.port },
		{ "AESKey", JsonSerializer::SerializeAES(relay.key) },
		{ "SessionID", relay.sessionID }
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

std::vector<unsigned char> JsonSerializer::SerializeRelayConnectionRequest(const unsigned int sessionID)
{
	std::vector<unsigned char> vec;
	
	std::string sigStr = "DIR" + std::to_string(static_cast<int>(DirectoryCodes::RelayConnection));
	const char* signature = sigStr.c_str();
	vec.insert(vec.begin(), signature, signature + sizeof(signature) - 1);

	json j;
	j["SessionID"] = sessionID;

	auto str = j.dump();

	for (const auto& i : str)
		vec.push_back(i);

	return vec;
}

std::vector<unsigned char> JsonSerializer::SerializeAES(const AES& aes)
{
	json j;
	std::vector<unsigned char> buffer;
	auto aesKey = aes.get_key();
	auto aesIV = aes.get_iv();

	std::stringstream ss1;
	std::stringstream ss2;

	ss1 << std::hex << std::setfill('0');
	ss2 << std::hex << std::setfill('0');
	
	for (size_t i = 0; i < sizeof(aesKey); ++i) 
	{
		ss1 << std::setw(2) << static_cast<int>(aesKey[i]);
		ss2 << std::setw(2) << static_cast<int>(aesIV[i]);
	}

	std::string hexKey = ss1.str();
	std::string hexIV = ss2.str();

	j["AESKey"] = hexKey;
	j["AESIV"] = hexIV;

	auto jsonString = j.dump();

	for (const auto& byte : jsonString)
		buffer.push_back(byte);

	return buffer;
}
