#include "MessageRequest.h"
#include "Relay.h"
#include <vector>


class JsonDecoder
{
public:
	static MessageRequest DecodeClientMessage(const std::vector<unsigned char>& message);
	static std::vector<Relay> DecodeGetRelaysResponse(const std::vector<unsigned char>& response);
};