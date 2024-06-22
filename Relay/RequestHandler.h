#include "Request.h"
#include <RSA.h>
#include <AES.h>
#include <iostream>

enum class DirRequests { ServeRequest };

class RequestHandler
{
public:
	static Request HandleRequest(std::vector<unsigned char>& data);
	static DirResponse HandleDirRequest(std::vector<unsigned char>& data);

private:
	// Directory
	static DirRequests DetermineDirRequest(std::vector<unsigned char>& data);
	static DirResponse HandleKeyRequest();
};
