#pragma once
#include "Relay.h"
#include "Directory.h"
#include <vector>
#include <fstream>
#include <string>

class NetworkHandler
{
public:
	NetworkHandler();
	bool IsConnected() const;

private:
	Directory GetNextDir() const;
	bool GetRelays();
	std::vector<unsigned char> GetRelayRequest();
	bool ReceiveRelays(SOCKET sock);
	bool DecodeMessage(const std::vector<unsigned char>& message);

	bool _isConnected;
	std::vector<Relay> _relays;
	Directory _dir;

	// Directory Finding
	const std::string _dirFileName = "dirlist.txt";
	std::ifstream _dirFile;
};