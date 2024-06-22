#pragma once
#include <vector>
#include <string>

struct MessageRequest
{
	std::vector<unsigned char> _data;
	std::string _destIP;
	MessageRequest() = default;
	MessageRequest(const std::vector<unsigned char>& data, const std::string& destIP) : _data(data), _destIP(destIP) {}
};
