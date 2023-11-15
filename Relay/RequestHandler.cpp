#include "RequestHandler.h"
#include "cryptopp/rsa.h"
#include "cryptopp/osrng.h"
#include "cryptopp/base64.h"

using namespace CryptoPP;

Request RequestHandler::HandleRequest(std::vector<unsigned char>& data)
{
	Request request;

	DecryptData(data);
	request.ip = ExtractIP(data);
	request.data = data;

	return request;
}

void RequestHandler::DecryptData(std::vector<unsigned char>& data)
{
//	DecryptRSA(data);
//	DecryptAES(data);
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

