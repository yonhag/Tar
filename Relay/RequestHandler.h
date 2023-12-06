#include "Request.h"
#include <iostream>

enum class DirRequests { Keys };

class RequestHandler
{
public:
	static Request HandleRequest(std::vector<unsigned char>& data);
	static DirResponse HandleDirRequest(std::vector<unsigned char>& data);
private:
	// Directory
	static DirRequests DetermineDirRequest(std::vector<unsigned char>& data);
	static DirResponse HandleKeyRequest();

	// Decryption
	static std::vector<unsigned char> DecryptData(const std::vector<unsigned char>& data);
	static std::vector<unsigned char> DecryptRSA(const std::vector<unsigned char>& data);
	static std::vector<unsigned char> DecryptAES(const std::vector<unsigned char>& data);

	static std::string ExtractIP(std::vector<unsigned char>& data);
	static std::array<uint8_t, 4> split_uint32_to_bytes(uint32_t input);
};