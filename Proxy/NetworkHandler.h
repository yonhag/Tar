#pragma once
#include "Relay.h"
#include "Directory.h"
#include <vector>
#include <fstream>

class NetworkHandler
{
public:
	NetworkHandler();
	NetworkHandler(const NetworkHandler& nwh);
	bool IsConnected() const;

	std::vector<unsigned char> EncryptMessage(const std::vector<unsigned char>& message);

private:
	// Network connection
	Directory GetNextDir();
	bool GetRelays();
	std::vector<unsigned char> GetRelayRequest();
	bool ReceiveRelays(SOCKET sock);
	bool DecodeConnectionMessage(const std::vector<unsigned char>& message);

	// Encryption
	static std::vector<unsigned char> EncryptAES(const std::vector<unsigned char>& message, const unsigned long key);
	static std::vector<unsigned char> EncryptRSA(const std::vector<unsigned char>& message, const unsigned long key);

	// Decryption

	bool _isConnected;
	std::vector<Relay> _relays;
	Directory _dir;

	// Directory Finding
	const std::string _dirFileName = "dirlist.txt";
	std::ifstream _dirFile;
};