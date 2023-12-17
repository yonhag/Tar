#pragma once
#include "Request.h"
#include "Response.h"

class RequestHandler
{
public:
	static Response HandleRequest(Request request);
};