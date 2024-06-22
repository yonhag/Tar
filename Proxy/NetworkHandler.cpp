#pragma once
#include "NetworkHandler.h"
#include "Communicator.h"
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include "Consts.h"
#include <iostream>

NetworkHandler::NetworkHandler(const LoadLevel loadlevel)
{
    std::ifstream dirFile(this->_dirFileName);

    if (!dirFile.is_open())
        throw std::exception("File not opening");

    bool hasFoundDir = false;
    while (!hasFoundDir && !dirFile.eof())
    {
        this->_dir = GetNextDir(dirFile);

        std::cout << "Trying " << this->_dir._ip << ":" << this->_dir._port << std::endl;

        hasFoundDir = GetRelays(loadlevel);
    }
    dirFile.close();

    if (hasFoundDir)
        this->_isConnected = true;
    else
        this->_isConnected = false;
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
        while (!hasFoundDir && !dirFile.eof())
        {
            this->_dir = GetNextDir(dirFile);
            hasFoundDir = GetRelays(loadlevel);
        }
        dirFile.close();

        if (hasFoundDir)
            this->_isConnected = true;
        else
            this->_isConnected = false;
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
    try 
    {
        std::vector<unsigned char> relayResponse = Communicator::GetRelays(this->_dir, loadlevel);
            
        for (auto& i : relayResponse)
            std::cout << i;
        std::cout << std::endl;

        this->_relays = JsonDeserializer::DeserializeGetRelaysResponse(relayResponse);
        
        return this->_relays.size() == 3;
    }
    catch (std::exception& e) 
    { 
        std::cout << e.what() << std::endl; 
        return false; 
    }
}

std::vector<unsigned char> NetworkHandler::EncryptMessage(const MessageRequest& message)
{
    // Adding the original destination for last relay to open
    MessageRequest currRequest = MessageRequest(message._data, message._destIP);
    std::vector<unsigned char> encrypted = JsonSerializer::SerializeRelayDataSendingRequest(currRequest);

    // Encrypting with last relay key
    auto key = this->_relays[this->_relays.size() - 1]._AESKey;
    encrypted = key.EncryptCBC(encrypted);

    // Adding the next relay for the rest of them
    for (int i = this->_relays.size() - 2; i >= 0; i--)
    {
        MessageRequest currRequest = MessageRequest(encrypted, this->_relays[i + 1]._ip);
        encrypted = JsonSerializer::SerializeRelayDataSendingRequest(currRequest);

        auto key = this->_relays[i]._AESKey;
        encrypted = key.EncryptCBC(encrypted);
    }

    return encrypted;
}

std::string NetworkHandler::GetFirstRelayIP() const
{
    return "127.0.0.1"; // #TODO: CHANGE THIS BACK TO return this->_relays[0]._ip; 
}

unsigned short NetworkHandler::GetFirstRelayPort() const
{
    return this->_relays[0]._port;
}
