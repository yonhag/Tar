#include "NetworkManager.h"
#include "Response.h"
#include "Communicator.h"
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include <random>
#include <algorithm>
#include <iostream>

// Initializing the relay vector
// Value can be changed for testing
std::vector<Relay> NetworkManager::_relays;
std::unordered_set<unsigned int> NetworkManager::_sessionIDs;

enum AssignedUserWeight { Low = 1, Medium = 2, High = 4 };

std::vector<DedicatedRelay> NetworkManager::GetRelays(const LoadLevel loadlevel)
{
    for (auto& i : _relays)
    {
        std::cout << i.ip << std::endl;
    }

    // Making sure there are enough relays avilable
    if (_relays.size() < 3)
    {
        std::cout << "Not enough relays" << std::endl;
        return std::vector<DedicatedRelay>();
    }
    std::vector<DedicatedRelay> dedicated_relays;

    if (loadlevel == LoadLevel::High)
    {
        for (size_t i = 0; i < relays_per_user; i++)
        {
            dedicated_relays.push_back(DedicateRelay(_relays[i]));
            _relays[i].assigned_users += AssignedUserWeight::High;
        }
    }
    else if (loadlevel == LoadLevel::Low)
    {
        std::cout << "low " << _relays.size() << std::endl;
        for (int i = _relays.size() - 1; i >= (_relays.size() - 3); i--)
        {
            try {
                std::cout << "IP: " << _relays[i].ip << " Port: " << _relays[i].listening_port << " Relays: " << _relays.size() << std::endl;

                DedicatedRelay drel = DedicateRelay(_relays[i]);

                // If ip is empty - relay didn't respond.
                // #TODO: Move to a different function
                if (drel.ip == "")
                {
                    std::cout << "Empty drel" << std::endl;
                    _relays.erase(_relays.begin() + i); // Removing unresponsive relay
                    if (_relays.size() > 3)
                        return GetRelays(loadlevel); // Trying again
                    else if (_relays.size() <= 3)
                        return std::vector<DedicatedRelay>();
                }

                dedicated_relays.push_back(drel);
                _relays[i].assigned_users += AssignedUserWeight::Low;

                if (dedicated_relays.size() == 3)
                    break;
            }
            catch (std::exception& e) { std::cout << e.what(); }
        }
    }
    else
        dedicated_relays = DedicateRelaysForNormalLoadUser();

    std::cout << "Relays:" << std::endl;
    for (auto& i : dedicated_relays)
        std::cout << " IP: " << i.ip;
    std::cout << std::endl << std::endl;

    return dedicated_relays;
}

void NetworkManager::JoinNetwork(const std::string& ip, const unsigned int bandwidth)
{
    Relay newRelay;
    newRelay.ip = ip;
    newRelay.bandwidth = bandwidth;
    
    AddRelay(newRelay);

    Communicator::UpdateOtherDirectories(JsonSerializer::SerializeUpdateDirectoryRequest(newRelay));
    // TODO: Add a check on function return value
}

bool NetworkManager::AddRelay(const Relay& relay)
{
    _relays.push_back(relay);

    // Sorted by bandwidth ascending - Lower bandwidth -> Higher banwidth
    std::sort(_relays.begin(), _relays.end(), [](const Relay& a, const Relay& b) { return a.bandwidth < b.bandwidth; });

    return true;
}

bool NetworkManager::RemoveRelay(const Relay& relay)
{
    int size = _relays.size();
    _relays.erase(std::remove_if(_relays.begin(), _relays.end(), [=](const Relay& rel) { return rel.ip == relay.ip && rel.bandwidth == relay.bandwidth; }));
    return size == _relays.size();
}

DedicatedRelay NetworkManager::DedicateRelay(const Relay& relay)
{
    std::cout << "Sending Request" << std::endl;
    Response response = Communicator::SendRelayConnectionRequest(relay, JsonSerializer::SerializeRelayConnectionRequest());
    
    DedicatedRelay drel;

    drel.ip = relay.ip;
    drel.port = relay.listening_port;
    drel.key = JsonDeserializer::DeserializeRelayDedicationResponse(response);

    return drel;
}

std::vector<DedicatedRelay> NetworkManager::DedicateRelaysForNormalLoadUser()
{
    std::vector<DedicatedRelay> dedicated_relays;

    // Setting the seed for randomizer
    std::random_device rd;
    std::mt19937 mt{ rd() };
    std::uniform_int_distribution<int> dist;

    if (_relays.size() > 10)
        // Relays - 7, removing 3 highest capacity, 3 lowest capacity, and 1 since index starts from 0
        std::uniform_int_distribution<int> dist{ 0, (static_cast<int>(_relays.size()) - 7) }; // Static cast is fine since number shouldn't be too high
    else
        std::uniform_int_distribution<int> dist{ 0, static_cast<int>(_relays.size() - 1) };
    
    // Dedicating the relays
    while (dedicated_relays.size() < 3)
    {
        int index = dist(mt);
        auto it = std::find(dedicated_relays.begin(), dedicated_relays.end(), _relays[index]);

        // If relay wasn't already dedicated
        if (it != dedicated_relays.end())
        {
            dedicated_relays.push_back(DedicateRelay(_relays[index]));
            _relays[index].assigned_users += AssignedUserWeight::Medium;
        }
    }

    return dedicated_relays;
}
