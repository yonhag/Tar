#pragma once
#include "Relay.h"
#include "Directory.h"
#include "MessageRequest.h"
#include "LoadLevel.h"
#include <fstream>
#include <SFML/Network.hpp>

class NetworkHandler
{
public:
	explicit NetworkHandler(const LoadLevel loadlevel);
	NetworkHandler(const NetworkHandler& nwh, const LoadLevel loadlevel);

	bool IsConnected() const;
	std::vector<unsigned char> EncryptMessage(const MessageRequest& message);
	std::string GetFirstRelayIP() const;

	// Helper functions

private:
	// Network connection
	Directory GetNextDir(std::ifstream& dirFile) const;
	bool GetRelays(LoadLevel loadlevel);

	// Encryption
	static std::vector<unsigned char> AddIP(const std::vector<unsigned char>& message, const std::string& ip);
	static std::vector<unsigned char> EncryptAES(const std::vector<unsigned char>& message, const unsigned long key);
	static std::vector<unsigned char> EncryptRSA(const std::vector<unsigned char>& message, const unsigned long key);

	// Decryption
	

	bool _isConnected;
	std::vector<Relay> _relays;
	Directory _dir;

	// Directory Finding
	const std::string _dirFileName = "dirlist.txt";
};