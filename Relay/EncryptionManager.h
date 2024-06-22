#pragma once
#include <AES.h>
#include <map>

class EncryptionManger
{
	std::vector<unsigned char> Decrypt(const std::vector<unsigned char>& message, const unsigned int sessionID);

	static std::map<unsigned int, AES> _sessions;
};