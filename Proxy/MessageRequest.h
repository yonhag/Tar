#pragma once
#include <vector>
#include <string>

struct MessageRequest
{
	std::vector<unsigned char> _data;
	std::string _destIP;
};