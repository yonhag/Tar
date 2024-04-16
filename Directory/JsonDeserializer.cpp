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
bool JsonDeserializer::DeserializeRelayDedicationResponse(const Response& response)
{
    if (response.data.size() != 2)
        return false;
    return response.data[0] == 'O' && response.data[1] == 'K';
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

RSA JsonDeserializer::DeserializeRSAKeyExchange(const std::vector<unsigned char>& message)
{
    json j = json::parse(message);

    PublicKey key = RSA::PlainToCipher(j["RSAKey"])[0]; // PTC returns an array, we only need one number from it - hence [0]
    Product prod = RSA::PlainToCipher(j["RSAProduct"])[0];

    return RSA(key, prod);
}
