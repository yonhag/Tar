#include "Request.h"
#include <iostream>

enum class DirRequests { Keys };

class RequestHandler
{
public:
	RequestHandler(const std::vector<unsigned char>& data);

	Request HandleRequest(std::vector<unsigned char>& data);
	DirResponse HandleDirRequest(std::vector<unsigned char>& data);

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
	static std::string ExtractIP(std::vector<unsigned char>& data);
};