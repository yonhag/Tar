#include "RequestHandler.h"
#include "JsonRequestDeserializer.h"

Response RequestHandler::HandleRequest(const std::vector<unsigned char>& message)
{
    Response response;
    Request request;

    if (message[request_type_index] == '1')
    {
        request = JsonRequestDeserializer::DeserializeGetRelaysRequests(message);
        response = 
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
