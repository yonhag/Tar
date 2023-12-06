#pragma once
#include "Relay.h"
#include "Directory.h"
#include <optional>
#include <vector>
#include <fstream>

class NetworkHandler
{
public:
	NetworkHandler();
	NetworkHandler(const NetworkHandler& nwh);
	bool IsConnected() const;
	
	std::optional<std::vector<unsigned char>> SendMessage(const std::vector<unsigned char>& message);

private:
	// Network connection
	Directory GetNextDir();
	bool GetRelays();
	std::vector<unsigned char> GetRelayRequest();
	bool ReceiveRelays(SOCKET sock);
	bool DecodeConnectionMessage(const std::vector<unsigned char>& message);

	// Network usage
	std::vector<unsigned char> EncryptMessage(const std::vector<unsigned char>& message);
	std::vector<unsigned char> EncryptAES(const std::vector<unsigned char>& message);
	std::vector<unsigned char> EncryptRSA(const std::vector<unsigned char>& message);

	bool _isConnected;
	std::vector<Relay> _relays;
	Directory _dir;

	// Directory Finding
	const std::string _dirFileName = "dirlist.txt";
	std::ifstream _dirFile;
};