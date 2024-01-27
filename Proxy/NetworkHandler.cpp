#include "JsonSerializer.h"
#include "NetworkHandler.h"
#include "JsonDeserializer.h"
#include "Consts.h"

NetworkHandler::NetworkHandler(const LoadLevel loadlevel)
{
    std::ifstream dirFile(this->_dirFileName);

    if (!dirFile.is_open())
        throw std::exception("File not opening");

    bool hasFoundDir = false;
    while (!hasFoundDir)
    {
        this->_dir = GetNextDir(dirFile);
        
        // If file is over
        if (dirFile.eof())
            throw std::exception("Reached EOF");

        hasFoundDir = GetRelays(loadlevel);
    }
    dirFile.close();
}

NetworkHandler::NetworkHandler(const NetworkHandler& nwh, const LoadLevel loadlevel) :
    _isConnected(nwh._isConnected), _relays(nwh._relays), _dir(nwh._dir)
{
    if (this->_relays.empty())
    {
        std::ifstream dirFile(this->_dirFileName);

        if (!dirFile.is_open())
            throw std::exception("File not opening");

        bool hasFoundDir = false;
        while (!hasFoundDir)
        {
            this->_dir = GetNextDir(dirFile);
            hasFoundDir = GetRelays(loadlevel);
        }
        dirFile.close();
    }
}

bool NetworkHandler::IsConnected() const
{
    return this->_isConnected;
}

Directory NetworkHandler::GetNextDir(std::ifstream& dirFile) const
{
    Directory dir;
    std::string line;

    std::getline(dirFile, line);

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

bool NetworkHandler::GetRelays(const LoadLevel loadlevel)
{
    
    return ReceiveRelays(sock);
}

bool NetworkHandler::ReceiveRelays(sf::TcpSocket sock)
{
    std::vector<unsigned char> message;

    return HandleConnectionMessage(message);
}

bool NetworkHandler::HandleConnectionMessage(const std::vector<unsigned char>& message)
{
    this->_relays = JsonDeserializer::DeserializeGetRelaysResponse(message);

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

std::string NetworkHandler::GetFirstRelayIP() const
{
    return this->_relays[0]._ip;
}

std::vector<unsigned char> NetworkHandler::AddIP(const std::vector<unsigned char>& message, const std::string& ip)
{
    std::vector<unsigned char> updatedMessage = message;

    for (int i = 0; i < ip_size; i++)
        updatedMessage.push_back(ip[i]);

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
