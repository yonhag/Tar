#include "Request.h"
#include <iostream>

enum class DirRequests { ServeRequest };

class RequestHandler
{
public:
	RequestHandler();

	Request HandleRequest(std::vector<unsigned char>& data);
	DirResponse HandleDirRequest(std::vector<unsigned char>& data) const;

private:
	// Directory
	DirRequests DetermineDirRequest(std::vector<unsigned char>& data) const;
	DirResponse HandleKeyRequest() const;
	
	// Decryption
	std::vector<unsigned char> DecryptData(const std::vector<unsigned char>& data);
	std::vector<unsigned char> DecryptRSA(const std::vector<unsigned char>& data);
	std::vector<unsigned char> DecryptAES(const std::vector<unsigned char>& data);

	// Members
	int _RSAKey;
	int _AESKey;

	// Additionals
};