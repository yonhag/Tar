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

private:
	Directory GetNextDir();
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