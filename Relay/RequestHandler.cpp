#include "RequestHandler.h"
#include "cryptopp/osrng.h"
#include "cryptopp/base64.h"
#include "cryptopp/hex.h"
#include "cryptopp/files.h"

#include "cryptopp/filters.h"
#include "cryptopp/strciphr.h"


Request RequestHandler::HandleRequest(std::vector<unsigned char>& data, const RSA::PrivateKey& key)
{
	Request request;

	DecryptData(data, key);
	request.ip = ExtractIP(data);
	request.data = data;

	return request;
}

std::vector<unsigned char> RequestHandler::DecryptData(const std::vector<unsigned char>& data, const RSA::PrivateKey& key)
{
	DecryptRSA(data, key);
	DecryptAES(data);
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

}

std::vector<unsigned char> RequestHandler::DecryptRSA(const std::vector<unsigned char>& data, const RSA::PrivateKey& key)
{
	std::vector<unsigned char> dercyptedText;
	const int StandardKeySize = 2048;

	// Creating the keys
	AutoSeededRandomPool prng;
	InvertibleRSAFunction params;

	params.GenerateRandomWithKeySize(prng, StandardKeySize);

	// ?
	RSA::PrivateKey key(params);

	RSAES_OAEP_SHA256_Encryptor d(key);
	HexEncoder encoder(new FileSink(std::cout));

	VectorSource(data, true, new PK_EncryptorFilter(prng, d, new VectorSink(dercyptedText)));

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
