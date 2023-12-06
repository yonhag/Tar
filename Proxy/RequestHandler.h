#pragma once
#include "Request.h"
#include <iostream>

class RequestHandler
{
public:
	RequestHandler(std::vector<unsigned char> handshake);

	Request HandleRequest(std::vector<unsigned char>& data);
private:
	// Decryption
	std::vector<unsigned char> DecryptData(const std::vector<unsigned char>& data);
	std::vector<unsigned char> DecryptRSA(const std::vector<unsigned char>& data);
	std::vector<unsigned char> DecryptAES(const std::vector<unsigned char>& data);


	int GetKeyFromHandshake(std::vector<unsigned char>& data); // int should be a key
	std::string ExtractIP(std::vector<unsigned char>& data);

	int RSAKey;
	int AESKey;
}; 
