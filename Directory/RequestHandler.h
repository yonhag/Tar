#pragma once
#include "Request.h"
#include "Response.h"
#include "DirectoryCodes.h"

class RequestHandler
{
public:
	static Response HandleRequest(const std::vector<unsigned char>& request);
	
private:
	static Response HandleDirRequest(const std::vector<unsigned char>& request);

	static Response SerializeResponse(const std::vector<unsigned char>& data);
	static void AddDirSignature(std::vector<unsigned char>& data);
	
	static bool IsDirMessage(const std::vector<unsigned char>& message);
	static DirectoryCodes DetermineDirRequest(const std::vector<unsigned char>& request);

	static bool AddRelay(const std::vector<unsigned char>& request);
	static bool RemoveRelay(const std::vector<unsigned char>& request);

	static std::vector<unsigned char> RemoveFirstCharsFromVector(const std::vector<unsigned char>& vec, const unsigned int amtofchars);

	static const int request_type_index = 0;
};