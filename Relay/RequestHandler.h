#include "Request.h"
#include "cryptopp/rsa.h"
#include "cryptopp/aes.h"
#include <iostream>

using namespace CryptoPP;

class RequestHandler
{
public:
	static Request HandleRequest(std::vector<unsigned char>& data, const RSA::PublicKey& key);
private:
	// Decryption
	static std::vector<unsigned char> DecryptData(const std::vector<unsigned char>& data, const RSA::PublicKey& key);
	static std::vector<unsigned char> DecryptRSA(const std::vector<unsigned char>& data, const RSA::PublicKey& key);
	static std::vector<unsigned char> DecryptAES(const std::vector<unsigned char>& data);

	static std::string ExtractIP(std::vector<unsigned char>& data);
};