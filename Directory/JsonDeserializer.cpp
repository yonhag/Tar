#include "JsonDeserializer.h"

using json = nlohmann::json;

void from_json(const json j, Relay& relay)
{
    relay.ip = j["IP"];
    relay.bandwidth = j["Bandwidth"];
}

LoadLevel JsonDeserializer::DeserializeGetRelaysRequest(const std::vector<unsigned char>& request)
{
    json j;
    j = json::parse(request);

    return j["LoadLevel"];
}

Relay JsonDeserializer::DeserializeRelayConnectionRequest(const std::vector<unsigned char>& request)
{
    Relay relay;

    json j = json::parse(request);

    relay.ip = j["IP"];
    relay.bandwidth = j["Bandwidth"];
    relay.listening_port = j["Port"];
    relay.assigned_users = 0; // No Users currently assigned

    return relay;
}

// Returns a dedicated relay, minus the IP
DedicatedRelay JsonDeserializer::DeserializeRelayDedicationResponse(const Response& response)
{
    DedicatedRelay relay;
    json j = json::parse(response.data);

    // TODO: Fix the key handling

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
