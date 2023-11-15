#include "cryptopp/rsa.h"
#include "Request.h"
#include <iostream>

class RequestHandler
{
public:
	static Request HandleRequest(std::vector<unsigned char> data);
};