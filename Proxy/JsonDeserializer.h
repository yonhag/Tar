#include "MessageRequest.h"
#include "Relay.h"
#include <vector>


class JsonDeserializer
{
public:
	static MessageRequest DeserializeClientMessage(const std::vector<unsigned char>& message);
	static std::vector<Relay> DeserializeGetRelaysResponse(const std::vector<unsigned char>& response);
	static AES DesierlizeRSAHandshake(const std::vector<unsigned char>& response);
};