#include "JsonDeserializer.h"

using json = nlohmann::json;

void from_json(const json j, Relay& relay)
{
    relay.ip = j["IP"];
    relay.bandwidth = j["Bandwidth"];
}

// Returns a dedicated relay, minus the IP
DedicatedRelay JsonDeserializer::DeserializeRelayConnectionResponse(const Response& response)
{
    DedicatedRelay relay;
    json j = json::parse(response.data);

    relay.AESKey = j["AESKey"];
    relay.RSAKey = j["RSAKey"];

    return relay;
}

bool JsonDeserializer::DeserializeUpdateDirectoriesResponse(const std::vector<unsigned char>& response)
{
    if (response.size() != 2)
        return false;
    return response[0] == 'O' && response[1] == 'K';
}

Relay JsonDeserializer::DeserializeUpdateDirectoryRequest(const std::vector<unsigned char>& request)
{
    Relay relay;

    json j = json::parse(request);

    relay = j["Relay"];

    return relay;
}
