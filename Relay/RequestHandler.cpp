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
	DecryptRSA(data);
	DecryptAES(data);
}