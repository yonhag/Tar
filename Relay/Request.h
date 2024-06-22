#pragma once
#include "Consts.h"
#include <vector>
#include <string>

struct Request
{
	std::string dest_ip;
	std::vector<unsigned char> data;
	unsigned int sessionID;
};

struct DirResponse
{
	std::vector<unsigned char> data;
};