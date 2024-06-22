#include "RequestHandler.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "EncryptionManager.h"
#include <array>

Request RequestHandler::HandleRequest(std::vector<unsigned char>& data)
{
	Request request = Deserializer::DeserializeClientMessages(data);

	return request;
}

std::vector<unsigned char> RequestHandler::HandleDirRequest(std::vector<unsigned char>& data)
{
	std::vector<unsigned char> request;
	switch (DetermineDirRequest(data))
	{
	case DirRequests::ServeRequest:
		return RequestHandler::HandleServeRequest(data);
	default:
		throw std::exception("Unknown Directory Code");
	}
	return request;
}

DirRequests RequestHandler::DetermineDirRequest(std::vector<unsigned char>& data)
{
	const int RequestTypeIndex = 4;

	std::cout << data[RequestTypeIndex] << std::endl;

	switch (data[RequestTypeIndex])
	{
	case '1':
		return DirRequests::ServeRequest;
	}
	return DirRequests();
}

std::vector<unsigned char> RequestHandler::HandleServeRequest(const std::vector<unsigned char>& data, const AES& aes)
{
	unsigned int id = Deserializer::DeserializeServeRequest(data);
	
	EncryptionManager::AddSession(id, aes);

	return Serializer::SerializeAES(aes);
}

std::vector<unsigned char> RequestHandler::DecryptData(const std::vector<unsigned char>& data)
{
	std::vector<unsigned char> decryptedData = DecryptRSA(data);
	decryptedData = DecryptAES(decryptedData);

	return decryptedData;
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
