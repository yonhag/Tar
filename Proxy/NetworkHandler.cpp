#include <WinSock2.h>
#include <Windows.h>
#include <Ws2tcpip.h>
#include "NetworkHandler.h"
#include "json.hpp"

using json = nlohmann::json;

NetworkHandler::NetworkHandler() :
    _dirFile(this->_dirFileName)
{
    if (!this->_dirFile.is_open())
        throw std::exception("File not opening");

    bool hasFoundDir = false;
    while (!hasFoundDir)
    {
        this->_dir = GetNextDir();
        hasFoundDir = GetRelays();
    }
}

NetworkHandler::NetworkHandler(const NetworkHandler& nwh) :
    _isConnected(nwh._isConnected), _relays(nwh._relays), _dir(nwh._dir), _dirFile(this->_dirFileName)
{

}

bool NetworkHandler::IsConnected() const
{
    return this->_isConnected;
}

Directory NetworkHandler::GetNextDir()
{
    Directory dir;
    std::string line;

    std::getline(this->_dirFile, line);

    // Extracting the ip
    bool isPort = false;
    std::string port = "";

    for (const char& ch : line)
    {
        if (ch == ' ')
            isPort = true;
        if (isPort)
            port += ch;
        else
            dir._ip += ch;
    }
    dir._port = std::stoi(port);

    return dir;
}

bool NetworkHandler::GetRelays()
{
    // Creating socket

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock = INVALID_SOCKET)
        throw std::exception("Invalid Socket Creation");

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->_dir._port);

    PCWSTR ip = StringToPCWSTR(this->_dir._ip);
    if (InetPton(AF_INET, ip, &(serverAddress.sin_addr)) != 1)
    {
        closesocket(sock);
        return false;
    }

    // Connecting
    if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
        throw std::exception("Connection Failed");

    // Sending
    std::vector<unsigned char> request = this->GetRelayRequest();
    if (send(sock, reinterpret_cast<const char*>(request.data()), request.size(), 0) == INVALID_SOCKET)
        return false;
    
    return ReceiveRelays(sock);
}

// Helper function
PCWSTR NetworkHandler::StringToPCWSTR(const std::string& str)
{
    // Convert std::string to wide string using the system's default code page
    int bufferSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    if (bufferSize == 0)
        throw std::exception("Error in StringToPCWSTR");

    std::wstring wideBuffer(bufferSize, L'\0');

    // Convert std::string to wide string
    if (MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wideBuffer[0], bufferSize) == 0)
        throw std::exception("Error in StringToPCWSTR");

    return wideBuffer.c_str();
}

std::vector<unsigned char> NetworkHandler::GetRelayRequest() const
{
    std::vector<unsigned char> request;
    request.push_back('1');

    return request;
}

bool NetworkHandler::ReceiveRelays(SOCKET sock)
{
    const int max_message_size = 2048;
    
    unsigned char buffer[max_message_size];
    int len = recv(sock, reinterpret_cast<char*>(buffer), max_message_size, NULL);
    
    try
    {
        ::closesocket(sock);
    }
    catch (...) {};

    if (len <= 0)
        return false;

    std::vector<unsigned char> message(buffer, buffer + len);

    return DecodeConnectionMessage(message);
}

bool NetworkHandler::DecodeConnectionMessage(const std::vector<unsigned char>& message)
{
    json j = json::parse(message);

    for (char i = '1'; i < '4'; i++)
    {
        Relay rel;
        rel._ip = j["IP" + i];
        rel._publicAESKey["AES" + i];
        rel._publicRSAKey["RSA" + i];

        this->_relays.push_back(rel);
    }

    return this->_relays.size() == 3;
}

std::vector<unsigned char> NetworkHandler::EncryptMessage(const MessageRequest& message)
{
    std::vector<unsigned char> encrypted = message._data;

    // Starting from the third, we add all the relay's IPs and encrypt with their keys.
    encrypted = AddIP(encrypted, message._destIP);
    for (int i = this->_relays.size(); i > 0; i--)
    {
        encrypted = EncryptAES(encrypted, this->_relays[i]._publicAESKey);
        encrypted = EncryptRSA(encrypted, this->_relays[i]._publicRSAKey);
        
        if (i != 1) // To not add the first relay
            encrypted = AddIP(encrypted, this->_relays[i]._ip);
    }

    return encrypted;
}

std::vector<unsigned char> NetworkHandler::AddIP(const std::vector<unsigned char>& message, const std::string& ip)
{
    std::vector<unsigned char> updatedMessage = message;



    return updatedMessage;
}

std::vector<unsigned char> NetworkHandler::EncryptAES(const std::vector<unsigned char>& message, const unsigned long key)
{
    std::vector<unsigned char> encrypted = message;
    return encrypted;
}

std::vector<unsigned char> NetworkHandler::EncryptRSA(const std::vector<unsigned char>& message, const unsigned long key)
{
    std::vector<unsigned char> encrypted = message;
    return encrypted;
}
