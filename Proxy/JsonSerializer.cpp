#include "JsonSerializer.h"
#include "json.hpp"
#include "DirRequestCodes.h"

using json = nlohmann::json;

/*
* Returns "0{"RSAKey"=<key>}"
*/
std::vector<unsigned char> JsonSerializer::SerializeRSAHandshake(const PublicKey& key, const Product& prod)
{
	json j;
	j["RSAKey"] = key.convert_to<std::string>();
	j["RSAProduct"] = prod.convert_to<std::string>();

	auto jsonString = j.dump();

	std::vector<unsigned char> buffer;
	for (const auto& byte : jsonString)
		buffer.push_back(byte);

	return buffer;
}

std::vector<unsigned char> JsonSerializer::SerializeGetRelaysRequest(const LoadLevel loadlevel)
{
	json j;

	j["LoadLevel"] = loadlevel;

	auto jsonString = j.dump();

	std::vector<unsigned char> buffer;
	buffer.push_back(DirRequestCodes::GetRelays);
	for (const auto& byte : jsonString)
		buffer.push_back(byte);

	return buffer;
}

std::vector<unsigned char> JsonSerializer::SerializeRelayDataSendingRequest(const MessageRequest& request)
{
	std::vector<unsigned char> message;

	json j;
	j["DestIP"] = request._destIP;
	j["Data"] = request._data;
	j["SessionID"] = request._sessionID;

	auto jsonString = j.dump();

	std::vector<unsigned char> buffer;
	for (const auto& byte : jsonString)
		buffer.push_back(byte);

	return buffer;
}
