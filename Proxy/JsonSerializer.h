#include "LoadLevel.h"
#include "MessageRequest.h"
#include <RSA.h>
#include <vector>

class JsonSerializer
{
public:
	static std::vector<unsigned char> SerializeRSAHandshake(const PublicKey& key, const Product& prod);
	static std::vector<unsigned char> SerializeGetRelaysRequest(const LoadLevel loadlevel);
	static std::vector<unsigned char> SerializeRelayDataSendingRequest(const MessageRequest& request);
};
