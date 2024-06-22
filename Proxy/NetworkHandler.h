#pragma once
#include "Session.h"
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

	std::vector<unsigned char> EncryptMessage(const MessageRequest& message);
	
	bool IsConnected() const;
	std::string GetFirstRelayIP() const;
	unsigned short GetFirstRelayPort() const;
	unsigned int GetSessionID() const;

private:
	// Network connection
	Directory GetNextDir(std::ifstream& dirFile) const;
	bool GetRelays(const LoadLevel loadlevel);	

	bool _isConnected;
	std::vector<Relay> _relays;
	Directory _dir;
	unsigned int _sessionID;

	// Directory Finding
	const std::string _dirFileName = "dirlist.txt";
};