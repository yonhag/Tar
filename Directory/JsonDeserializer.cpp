#include "JsonDeserializer.h"

using json = nlohmann::json;

LoadLevel JsonDeserializer::DeserializeGetRelaysRequest(const std::vector<unsigned char>& request)
{
    json j;
    j = json::parse(request);

    return j["LoadLevel"];
}

// Returns a dedicated relay, minus the IP
DedicatedRelay JsonDeserializer::DeserializeRelayConnectionResponse(const Response& response)
{
    DedicatedRelay relay;
    json j;

    j = json::parse(response.data);

    relay.AESKey = j["AESKey"];
    relay.RSAKey = j["RSAKey"];

    return relay;
}
