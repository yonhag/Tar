#include "RequestHandler.h"
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include "NetworkManager.h"

Response RequestHandler::HandleRequest(const std::vector<unsigned char>& message)
{
    Response response;
    
    if (message[request_type_index] == '1')
    {
        LoadLevel llevel = JsonDeserializer::DeserializeGetRelaysRequest(message);
        std::vector<DedicatedRelay> relays = NetworkManager::GetRelays(llevel);
        response = JsonSerializer::SerializeGetRelaysResponse(relays);
    }

    // TODO: 
    // Encrypt the response using the key

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
