#pragma once
#include <RSA.h>
#include <AES.h>
#include <string>

struct Directory
{
	std::string _ip;
	unsigned short _port;
	RSA _rsa;
	AES _aes;
};
