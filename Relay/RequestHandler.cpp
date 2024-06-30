#include "RequestHandler.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "EncryptionManager.h"
#include <array>

std::vector<unsigned char> DirRequestHandler::HandleDirRequest(std::vector<unsigned char>& data)
{
	std::vector<unsigned char> request;
	switch (DetermineDirRequest(data))
	{
	case DirRequests::ServeRequest:
		return DirRequestHandler::HandleServeRequest(data);
	default:
		throw std::runtime_error("Unknown Directory Code");
	}
	return request;
}

DirRequests DirRequestHandler::DetermineDirRequest(std::vector<unsigned char>& data)
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

std::vector<unsigned char> DirRequestHandler::HandleServeRequest(const std::vector<unsigned char>& data)
{
	unsigned int id = Deserializer::DeserializeServeRequest(data);
	
	AES aes;
	EncryptionManager::AddSession(id, aes);

	return Serializer::SerializeAES(aes);
}
