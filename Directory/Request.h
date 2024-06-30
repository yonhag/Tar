#pragma once
#include <vector>

struct Request
{
	int key;
	std::vector<unsigned char> data;
};