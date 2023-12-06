#include "Request.h"
#include <iostream>

enum class DirRequests { Keys };

class RequestHandler
{
public:
	RequestHandler();

	Request HandleRequest(std::vector<unsigned char>& data);
	DirResponse HandleDirRequest(std::vector<unsigned char>& data);

private:
	// Directory
	DirRequests DetermineDirRequest(std::vector<unsigned char>& data);
	DirResponse HandleKeyRequest();

	// Decryption
	std::vector<unsigned char> DecryptData(const std::vector<unsigned char>& data);
	std::vector<unsigned char> DecryptRSA(const std::vector<unsigned char>& data);
	std::vector<unsigned char> DecryptAES(const std::vector<unsigned char>& data);

	int RSAKey;
	int AESKey;

	// Additionals
	static std::string ExtractIP(std::vector<unsigned char>& data);
	static std::array<uint8_t, 4> split_uint32_to_bytes(uint32_t input);
};