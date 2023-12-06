#include <WinSock2.h>
#include <Windows.h>
#include <Ws2tcpip.h>
#include <codecvt>
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

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    PCWSTR ip = converter.from_bytes(this->_dir._ip).c_str();
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

std::vector<unsigned char> NetworkHandler::GetRelayRequest()
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

std::vector<unsigned char> NetworkHandler::EncryptMessage(const std::vector<unsigned char>& message)
{
    std::vector<unsigned char> encrypted;

    for (const auto& relay : this->_relays)
    {
        encrypted = AddIP(encrypted);
        encrypted = EncryptAES(encrypted, relay._publicAESKey);
        encrypted = EncryptRSA(encrypted, relay._publicRSAKey);
    }

    return encrypted;
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
