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

Request Deserializer::DeserializeClientMessages(const std::vector<unsigned char>& data)
{
	Request request;
	
	json j;
	j = json::parse(data);
	request.dest_ip = j["DestIP"];
	request.sessionID = j["SessionID"];
	request.data = std::vector<unsigned char>(j["Data"]);

	return request;
}

RSA Deserializer::DeserializeReceivedRSAKeyExchange(std::vector<unsigned char>& message)
{
	json j = json::parse(message);

	PublicKey key = RSA::PlainToCipher(j["RSAKey"])[0]; // PTC returns an array, we only need one number from it - hence [0]
	Product prod = RSA::PlainToCipher(j["RSAProduct"])[0];

	return RSA(key, prod);
}

AES Deserializer::DeserializeRSAKeyExchangeInitiation(std::vector<unsigned char>& DecipheredResponse)
{
	const int AES_128_key_size = 128;
	json j = json::parse(DecipheredResponse);

	unsigned char key[AES_128_key_size];
	unsigned char iv[AES_128_key_size];

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
