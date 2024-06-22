#include "EncryptionManager.h"

std::map<unsigned int, AES> EncryptionManager::_sessions;

void EncryptionManager::AddSession(const unsigned int id, const AES& aes)
{
	_sessions.insert(std::pair<unsigned int, AES>(id, aes));
}

std::vector<unsigned char> EncryptionManager::Decrypt(const std::vector<unsigned char>& message, const unsigned int sessionID)
{
	AES aes = _sessions.find(sessionID)->second;

	return aes.DecryptCBC(message);
}
