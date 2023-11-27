#include <WinSock2.h>
#include <Windows.h>
#include "NetworkHandler.h"
#include "WSAInitializer.h"
#include "json.hpp"

using json = nlohmann::json;

NetworkHandler::NetworkHandler() :
    _dirFile(this->_dirFileName, 'r')
{
    bool hasFoundDir = false;
    while (!hasFoundDir)
    {
        this->_dir = GetNextDir();
        hasFoundDir = GetRelays();
    }
    
}

bool NetworkHandler::IsConnected() const
{
    return this->_isConnected;
}

Directory NetworkHandler::GetNextDir() const
{
    Directory dir{ nullptr };
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
    WSAInitializer wsa_init;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock = INVALID_SOCKET)
        throw std::exception("Invalid Socket Creation");

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->_dir._port);
    serverAddress.sin_addr.s_addr = inet_addr(this->_dir._ip.c_str());

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

    return DecodeMessage(message);
}

bool NetworkHandler::DecodeMessage(const std::vector<unsigned char>& message)
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
