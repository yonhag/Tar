#pragma once
#include "Consts.h"
#include <vector>
#include <string>

struct Request
{
	std::string ip;
	std::vector<unsigned char> data;
};