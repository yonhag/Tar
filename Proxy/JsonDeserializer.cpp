#include "JsonDeserializer.h"

using json = nlohmann::json;

void from_json(const json& j, Relay& relay)
{
	relay._ip = j["IP"];
	relay._port = j["Port"];
}

Session JsonDeserializer::DeserializeGetRelaysResponse(const std::vector<unsigned char>& response)
{
	Session session;
	std::vector<Relay> relays;
	
	json j = json::parse(response);
	
	for (int i = 1; i <= 3; i++)
		relays.push_back(j["Relay" + std::to_string(i)]);

	session._relays = relays;
	session._id = j["SessionID"];

	return session;
}

AES JsonDeserializer::DeserializeRSAHandshake(const std::vector<unsigned char>& DecipheredResponse)
{
	const int AES_128_key_size = 128;
	json j = json::parse(DecipheredResponse);

	unsigned char key[AES_128_key_size]{};
	unsigned char iv[AES_128_key_size]{};

	std::string storedKey = j["AESKey"];
	std::string storedIV = j["AESIV"];

	// Retreiving the keys
	for (size_t i = 0; i < AES_128_key_size; i += 2) 
	{
		key[i / 2] = static_cast<unsigned char>(std::stoi(storedKey.substr(i, 2), nullptr, 16));
		key[i / 2] = static_cast<unsigned char>(std::stoi(storedIV.substr(i, 2), nullptr, 16));
	}

	return AES(key, iv, AESKeyLength::AES_128);
}
