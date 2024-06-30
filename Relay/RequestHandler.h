#include "Request.h"
#include <RSA.h>
#include <AES.h>
#include <iostream>

enum class DirRequests { ServeRequest };

class DirRequestHandler
{
public:
	static std::vector<unsigned char> HandleDirRequest(std::vector<unsigned char>& data);

private:
	// Directory
	static DirRequests DetermineDirRequest(std::vector<unsigned char>& data);
	static std::vector<unsigned char> HandleServeRequest(const std::vector<unsigned char>& data);
};
