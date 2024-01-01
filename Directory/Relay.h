#pragma once
#include <string>

struct Relay
{
	std::string ip;
	unsigned int bandwidth; // Measured in Mbps
	unsigned int assigned_users; // Low = 1, Medium = 2, High = 4d
};