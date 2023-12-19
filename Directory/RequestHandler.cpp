#include "RequestHandler.h"
#include "JsonResponseSerializer.h"
#include "NetworkManager.h"

Response RequestHandler::HandleRequest(const std::vector<unsigned char>& message)
{
    Response response;
    
    if (message[request_type_index] == '1')
    {
         std::vector<DedicatedRelay> relays = NetworkManager::GetRelays();
         response = JsonResponseSerializer::SerializeGetRelaysResponse(relays);
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
