#include "MessageRequest.h"
#include "Relay.h"
#include "AES.h"
#include "Session.h"
#include <memory>
#include "json.hpp"


class JsonDeserializer
{
public:
	static MessageRequest DeserializeClientMessage(const std::vector<unsigned char>& message);
	static Session DeserializeGetRelaysResponse(const std::vector<unsigned char>& response);
	static AES DeserializeRSAHandshake(const std::vector<unsigned char>& response);
};
