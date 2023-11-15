#include "Request.h"
#include <iostream>

class RequestHandler
{
public:
	static Request HandleRequest(std::vector<unsigned char>& data);
private:
	// Decryption
	static void DecryptData(std::vector<unsigned char>& data);
	static void DecryptRSA(std::vector<unsigned char>& data);
	static void DecryptAES(std::vector<unsigned char>& data);

	static std::string ExtractIP(std::vector<unsigned char>& data);
};