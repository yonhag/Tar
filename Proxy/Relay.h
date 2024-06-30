#pragma once
#include <string>
#include <AES.h>

struct Relay
{
	std::string _ip;
	unsigned short _port;
	AES _AESKey;
};