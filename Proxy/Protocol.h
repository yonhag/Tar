#include <vector>
#include <iostream>
class Protocol
{
enum CODES{
	C_OK = 1,
	C_ERROR = 999,
	C_GET_DIR = 150,
	C_GET_KEYS = 200,
	C_GET_RELAYS =300
};

private:
	std::vector<unsigned char>& ProtocolMaker(std::vector<unsigned char>& data, CODES msg_code);
};