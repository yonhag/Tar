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
AES JsonDeserializer::DeserializeRelayDedicationResponse(const Response& response)
{
    const int AES_128_key_size = 16; // Measured in bytes
    json j = json::parse(response.data);

    unsigned char key[AES_128_key_size];
    unsigned char iv[AES_128_key_size];

    std::string storedKey = j["AESKey"];
    std::string storedIV = j["AESIV"];

    // Retreiving the keys
    for (size_t i = 0; i < AES_128_key_size; i += 2)
    {
        key[i / 2] = static_cast<unsigned char>(std::stoi(storedKey.substr(i, 2), nullptr, 16));
        key[i / 2] = static_cast<unsigned char>(std::stoi(storedIV.substr(i, 2), nullptr, 16));
    }

    return AES(key, iv, AESKeyLength::AES_128);
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

    // Convert strings to vectors of unsigned char
    std::string rsaKeyString = j["RSAKey"].get<std::string>();
    std::string rsaProductString = j["RSAProduct"].get<std::string>();

    Integer key(rsaKeyString);
    Integer prod(rsaProductString);

    std::cout << "Key: " << key << std::endl;

    return RSA(key, prod);
}
