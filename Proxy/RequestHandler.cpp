#include "RequestHandler.h"


RequestHandler::RequestHandler(std::vector<unsigned char> handshake)
{
	GetKeyFromHandshake(handshake);
}

Request RequestHandler::HandleRequest(std::vector<unsigned char>& data)
{
	Request request;

	DecryptData(data);
	request.dest_ip = ExtractIP(data);
	request.data = data;

	return request;
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

std::vector<unsigned char> RequestHandler::DecryptAES(const std::vector<unsigned char>& data)
{
	return std::vector<unsigned char>();
}

int RequestHandler::GetKeyFromHandshake(std::vector<unsigned char>& data)
{
	// Should assign both RSA and AES keys
	return 0;
}

std::vector<unsigned char> RequestHandler::DecryptRSA(const std::vector<unsigned char>& data)
{
	const int StandardKeySize = 2048;
	std::vector<unsigned char> dercyptedText;

	return dercyptedText;
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
