#include "MessageRequest.h"
#include "Relay.h"
#include "AES.h"
#include <vector>
#include <memory>
#include "json.hpp"


class JsonDeserializer
{
public:
	static MessageRequest DeserializeClientMessage(const std::vector<unsigned char>& message);
	static std::vector<Relay> DeserializeGetRelaysResponse(const std::vector<unsigned char>& response);
	static AES DeserializeRSAHandshake(const std::vector<unsigned char>& response);
};
