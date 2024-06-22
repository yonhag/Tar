#pragma once
#include <vector>
#include <string>

struct MessageRequest
{
	std::vector<unsigned char> _data;
	std::string _destIP;
	unsigned int _sessionID;

	MessageRequest() = default;
	MessageRequest(const std::vector<unsigned char>& data, const std::string& destIP, const unsigned int id) 
		: _data(data), _destIP(destIP), _sessionID(id) {}
};
