#pragma once
#include <AES.h>
#include <map>

class EncryptionManager
{
public:
	static void AddSession(const unsigned int id, const AES& aes);
	static std::vector<unsigned char> Decrypt(const std::vector<unsigned char>& message, const unsigned int sessionID);

private:
	static std::map<unsigned int, AES> _sessions;
};