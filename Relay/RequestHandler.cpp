#include "RequestHandler.h"
#include "JsonDeserializer.h"
#include "JsonSerializer.h"
#include <array>

RequestHandler::RequestHandler(const std::vector<unsigned char>& data)
{
	JsonDeserializer::DeserializeDirectoryConnectionRequest(data, this->_RSAKey, this->_AESKey);
}

Request RequestHandler::HandleRequest(std::vector<unsigned char>& data)
{
	Request request;

	DecryptData(data);
	request.dest_ip = ExtractIP(data);
	request.data = data;

	return request;
}

DirResponse RequestHandler::HandleDirRequest(std::vector<unsigned char>& data) const
{
	DirResponse request;
	switch (DetermineDirRequest(data))
	{
	case DirRequests::Keys:
		return HandleKeyRequest();
	}
	return request;
}

DirRequests RequestHandler::DetermineDirRequest(std::vector<unsigned char>& data) const
{
	const int RequestTypeIndex = 4;

	switch (data[RequestTypeIndex])
	{
	case '1':
		return DirRequests::Keys;
	}
	return DirRequests();
}

DirResponse RequestHandler::HandleKeyRequest() const
{
	DirResponse res;

	res.data = JsonSerializer::SerializeDirectoryConnectionResponse(this->_AESKey, this->_RSAKey);

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

	int startIndex = data.size() - ip_size;
	for (int i = 0; i < 15 && !data.empty(); i++)
	{
		ip += data[startIndex];
		data.erase(data.begin() + startIndex);
	}

	// Removing the padding
	for (int i = 0; i < ip_size && ip.size() > i; i++)
	{
		if (ip[i] == ',')
		{
			ip.erase(i, 1);
			--i;
		}
	}

	return ip;
}

std::vector<unsigned char> RequestHandler::DecryptAES(const std::vector<unsigned char>& data)
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
