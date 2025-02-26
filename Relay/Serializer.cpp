#include "Serializer.h"
#include "json.hpp"

using json = nlohmann::json;

std::vector<unsigned char> Serializer::SerializeAES(const AES& aes)
{
	json j;
	std::vector<unsigned char> buffer;
	auto aesKey = aes.get_key();

	std::stringstream ss1;
	std::stringstream ss2;

	ss1 << std::hex << std::setfill('0');
	ss2 << std::hex << std::setfill('0');

	for (size_t i = 0; i < sizeof(aesKey); ++i)
	{
		ss1 << std::setw(2) << static_cast<int>(aesKey[i]);
		ss2 << std::setw(2) << static_cast<int>(aesKey[i]);
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

std::vector<unsigned char> Serializer::SerializeDirectoryConnectionRequest(const std::string& ip, const unsigned int BandwidthInMb, const unsigned short listeningPort)
{
	std::vector<unsigned char> request;

	json j;
	j["IP"] = ip; 
	j["Bandwidth"] = BandwidthInMb;
	j["Port"] = listeningPort;

	auto dump = j.dump();

	request.push_back('2'); // Adding signature

	for (auto& byte : dump)
		request.push_back(byte);

	return request;
}

std::vector<unsigned char> Serializer::SerializeRSAKeyExchangeInitiation(const PublicKey& key, const Product& prod)
{
	std::cout << "Started Ser " << key << " and " << prod;
	std::cout << "k";
	json j;
	std::cout << "k";
	std::ostringstream oss;
	std::cout << "k";
	oss << key.str();
	std::cout << "k";
	j["RSAKey"] = oss.str();
	oss.clear();
	oss << prod.str();
	j["RSAProduct"] = oss.str();

	std::cout << "2";

	auto jsonString = j.dump();

	std::vector<unsigned char> buffer;
	for (const auto& byte : jsonString)
		buffer.push_back(byte);

	return buffer;
}
