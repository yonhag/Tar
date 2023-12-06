#include "JsonDecoder.h"
#include "json.hpp"
#include <codecvt>

using json = nlohmann::json;


MessageRequest JsonDecoder::DecodeClientMessage(const std::vector<unsigned char>& message)
{
	MessageRequest request;

	json j = json::parse(message);
	request._destIP = j["DestIP"];

	std::string data = j["Data"];
	request._data = std::vector<unsigned char>(data.begin(), data.end());

	return request;
}
