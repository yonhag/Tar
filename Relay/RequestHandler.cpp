#include "RequestHandler.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "EncryptionManager.h"
#include <array>

Request RequestHandler::HandleRequest(std::vector<unsigned char>& data)
{
	Request request = Deserializer::DeserializeClientMessages(data);

	return request;
}

std::vector<unsigned char> RequestHandler::HandleDirRequest(std::vector<unsigned char>& data)
{
	std::vector<unsigned char> request;
	switch (DetermineDirRequest(data))
	{
	case DirRequests::ServeRequest:
		return RequestHandler::HandleServeRequest(data);
	default:
		throw std::exception("Unknown Directory Code");
	}
	return request;
}

DirRequests RequestHandler::DetermineDirRequest(std::vector<unsigned char>& data)
{
	const int RequestTypeIndex = 4;

	std::cout << data[RequestTypeIndex] << std::endl;

	switch (data[RequestTypeIndex])
	{
	case '1':
		return DirRequests::ServeRequest;
	}
	return DirRequests();
}

std::vector<unsigned char> RequestHandler::HandleServeRequest(const std::vector<unsigned char>& data)
{
	unsigned int id = Deserializer::DeserializeServeRequest(data);
	
	AES aes;
	EncryptionManager::AddSession(id, aes);

	return Serializer::SerializeAES(aes);
}
