#include "RequestHandler.h"
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include "NetworkManager.h"
#include "RSA.h"
#include "AES.h"
#include <iostream> // TODO: Remove this

enum RequestCodes { GetRelays = '1', NewRelay = '2' };

Response RequestHandler::HandleRequest(const std::vector<unsigned char>& request)
{
    Response response;
    
    if (IsDirMessage(request))
        response = HandleDirRequest(request);

    std::cout << (request[request_type_index] == RequestCodes::NewRelay) << std::endl;

    if (request[request_type_index] == RequestCodes::GetRelays)
    {
        LoadLevel llevel = JsonDeserializer::DeserializeGetRelaysRequest(RemoveFirstCharsFromVector(request, 1));
        auto relays = NetworkManager::GetRelays(llevel);
        response = JsonSerializer::SerializeGetRelaysResponse(relays);
    }

    else if (request[request_type_index] == RequestCodes::NewRelay)
    {
        Relay newRelay = JsonDeserializer::DeserializeRelayConnectionRequest(RemoveFirstCharsFromVector(request, 1));
        std::cout << "passed" << std::endl;
        // TODO: Use JoinNetwork();
        NetworkManager::AddRelay(newRelay);
        
        response.data = std::vector<unsigned char>();
        response.data.push_back('O');
        response.data.push_back('K');
    }

    return response;
}

Response RequestHandler::HandleDirRequest(const std::vector<unsigned char>& request)
{
    Response response;
    bool done = false;
    DirectoryCodes code = DetermineDirRequest(request);

    if (code == DirectoryCodes::AddRelay)
        done = AddRelay(request);
    else if (code == DirectoryCodes::RemoveRelay)
        done = RemoveRelay(request);
    else if (code == DirectoryCodes::JoinNetwork)
    {
        NULL;
    }

    // Returning
    if (done)
    {
        std::string ok = "OK";
        response.data = std::vector<unsigned char>(ok.begin(), ok.end());
    }
    else
    {
        std::string no = "NO";
        response.data = std::vector<unsigned char>(no.begin(), no.end());
    }

    return response;
}

Response RequestHandler::SerializeResponse(const std::vector<unsigned char>& data)
{
    Response res;
    
    res.data = data;
    RequestHandler::AddDirSignature(res.data);

    return res;
}

void RequestHandler::AddDirSignature(std::vector<unsigned char>& data)
{
    data.insert(data.begin(), {'D', 'I', 'R'});
}

bool RequestHandler::IsDirMessage(const std::vector<unsigned char>& message)
{
    if (message[0] == 'D' && message[1] == 'I' && message[2] == 'R')
        return true;
    return false;
}

DirectoryCodes RequestHandler::DetermineDirRequest(const std::vector<unsigned char>& request)
{
    const int dir_request_type_index = 4;
    return (DirectoryCodes)(request[dir_request_type_index] - '0');
}

bool RequestHandler::AddRelay(const std::vector<unsigned char>& request)
{
    Relay relay = JsonDeserializer::DeserializeUpdateDirectoryRequest(request);
    return NetworkManager::AddRelay(relay);
}

bool RequestHandler::RemoveRelay(const std::vector<unsigned char>& request)
{
    Relay relay = JsonDeserializer::DeserializeUpdateDirectoryRequest(request);
    return NetworkManager::RemoveRelay(relay);
}

// Retruns vec if amtofchars > vec.size;
std::vector<unsigned char> RequestHandler::RemoveFirstCharsFromVector(const std::vector<unsigned char>& vec, const unsigned int amtofchars)
{
    if (amtofchars > vec.size())
        return vec;
    return std::vector<unsigned char>(vec.begin() + amtofchars, vec.end());
}
