#pragma once
#include "../Encryptions/RSA/head/RSA.h"
#include <string>

struct Directory
{
	std::string _ip;
	unsigned short _port;
	RSA _rsa;
	//AES _aes;
};
