#pragma once
#include "Relay.h"
#include "Directory.h"
#include "MessageRequest.h"
#include <fstream>

class NetworkHandler
{
public:
	NetworkHandler();
	NetworkHandler(const NetworkHandler& nwh);

	bool IsConnected() const;
	std::vector<unsigned char> EncryptMessage(const MessageRequest& message);
	std::string d() const;

	// Helper functions
	static PCWSTR StringToPCWSTR(const std::string& str);

private:
	// Network connection
	Directory GetNextDir(std::ifstream& dirFile) const;
	bool GetRelays();
	std::vector<unsigned char> GetRelayRequest() const; 
	bool ReceiveRelays(SOCKET sock);
	bool HandleConnectionMessage(const std::vector<unsigned char>& message);

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