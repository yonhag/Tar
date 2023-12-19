#pragma once
#include "Request.h"
#include "Response.h"

class RequestHandler
{
public:
	static Response HandleRequest(const std::vector<unsigned char>& request);
	
private:
	static Response SerializeResponse(const std::vector<unsigned char>& data);
	static void AddDirSignature(std::vector<unsigned char>& data);

	static const int request_type_index = 0;
};