#include "RequestHandler.h"
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include "NetworkManager.h"

Response RequestHandler::HandleRequest(const std::vector<unsigned char>& request)
{
    Response response;
    
    if (IsDirMessage(request))
        response = HandleDirRequest(request);
    if (request[request_type_index] == '1')
    {
        LoadLevel llevel = JsonDeserializer::DeserializeGetRelaysRequest(message);
        std::vector<DedicatedRelay> relays = NetworkManager::GetRelays(llevel);
        response = JsonSerializer::SerializeGetRelaysResponse(relays);
    }

    // TODO: 
    // Encrypt the response using the key

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
