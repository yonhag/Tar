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
    return "127.0.0.1"; // #TODO: CHANGE THIS BACK TO return this->_relays[0]._ip; 
}

unsigned short NetworkHandler::GetFirstRelayPort() const
{
    return this->_relays[0]._port;
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
