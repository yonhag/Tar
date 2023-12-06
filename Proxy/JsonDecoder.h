#include "MessageRequest.h"
#include <vector>


class JsonDecoder
{
public:
	static MessageRequest DecodeClientMessage(const std::vector<unsigned char>& message);
};