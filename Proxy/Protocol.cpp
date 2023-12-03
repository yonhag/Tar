#include "Protocol.h"
#include <string>

std::vector<unsigned char>& Protocol::ProtocolMaker(std::vector<unsigned char>& data, CODES msg_code)
{
	std::string pro_num = std::to_string(msg_code) + ':';
	data.insert(data.begin(), pro_num.begin(), pro_num.end());
	return data;
}