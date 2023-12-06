#include "RequestHandler.h"
#include "json.hpp"

using json = nlohmann::json;
using nlohmann::json;

Request RequestHandler::HandleRequest(std::vector<unsigned char>& data)
{
	Request request;

	DecryptData(data);
	request.dest_ip = ExtractIP(data);
	request.data = data;

	return request;
}

DirResponse RequestHandler::HandleDirRequest(std::vector<unsigned char>& data)
{
	DirResponse request;
	switch (DetermineDirRequest(data))
	{
	case DirRequests::Keys:
		return HandleKeyRequest();
	}
	return request;
}

DirRequests RequestHandler::DetermineDirRequest(std::vector<unsigned char>& data)
{
	const int RequestTypeIndex = 4;

	switch (data[RequestTypeIndex])
	{
	case '1':
		return DirRequests::Keys;
	}
}

DirResponse RequestHandler::HandleKeyRequest()
{
	DirResponse res;

	res.data = std::vector<unsigned char>();

	json j;
	
	auto jsonString = j.dump();
	
	// ADD INFO

	auto stringSizeInBytes = split_uint32_to_bytes(jsonString.size());
	for (const auto& byte : stringSizeInBytes)
		res.data.push_back(byte);

	for (const auto& byte : jsonString)
		res.data.push_back(byte);

	return res;
}

std::vector<unsigned char> RequestHandler::DecryptData(const std::vector<unsigned char>& data)
{
	std::vector<unsigned char> decryptedData = DecryptRSA(data);
	decryptedData = DecryptAES(decryptedData);

	return decryptedData;
}

std::string RequestHandler::ExtractIP(std::vector<unsigned char>& data)
{
	std::string ip = "";

	// Extracting the ip
	for (int i = 0; i < ip_size && !data.empty(); i++)
	{
		ip += data[0];
		data.erase(data.begin());
	}

	// Removing the paddding
	for (const char& i : ip)
	{
		if (i == ip_padding)
			ip.erase(ip.begin());
		else
			break;
	}

	return ip;
}

std::vector<unsigned char> RequestHandler::DecryptAES(const std::vector<unsigned char>& datadd)
{
	return std::vector<unsigned char>();
}

std::vector<unsigned char> RequestHandler::DecryptRSA(const std::vector<unsigned char>& data)
{
	return std::vector<unsigned char>();
}

std::string hex_to_string(const std::string& input)
{
	static const char* const hexChars = "0123456789ABCDEF";
	size_t len = input.length();
	if (len & 1) throw std::invalid_argument("odd length");

	std::string output;
	output.reserve(len / 2);
	for (size_t i = 0; i < len; i += 2)
	{
		char a = input[i];
		const char* p = std::lower_bound(hexChars, hexChars + 16, a);
		if (*p != a) throw std::invalid_argument("not a hex digit");

		char b = input[i + 1];
		const char* q = std::lower_bound(hexChars, hexChars + 16, b);
		if (*q != b) throw std::invalid_argument("not a hex digit");

		output.push_back(((p - hexChars) << 4) | (q - hexChars));
	}
	return output;
}

std::array<uint8_t, 4> RequestHandler::split_uint32_to_bytes(uint32_t input)
{
	std::array<uint8_t, 4> bytes;
	for (int i = 0; i < 4; i++) {
		bytes[i] = (input >> (i * 8)) & 0xFF;
	}
	return bytes;
}
