#include "NetworkManager.h"
#include "Response.h"
#include "Communicator.h"
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include <algorithm>

// Initializing the relay vector
// Value can be changed for testing
std::vector<Relay> NetworkManager::_relays;

enum AssignedUserWeight { Low = 1, Medium = 2, High = 4 };

std::vector<DedicatedRelay> NetworkManager::GetRelays(const LoadLevel loadlevel)
{
    // Making sure there are enough relays avilable
    if (_relays.size() < 3)
        return std::vector<DedicatedRelay>();

    std::vector<DedicatedRelay> relays;
    


    if (loadlevel == LoadLevel::High)
    {
        for (size_t i = 0; i < relays_per_user; i++)
        {
            relays.push_back(DedicateRelay(_relays[i]));
            _relays[i].assigned_users += AssignedUserWeight::High;
        }
    }
    else if (loadlevel == LoadLevel::Low)
    {
        for (size_t i = _relays.size() - 1; i < _relays.size() - 4; i++)
        {
            relays.push_back(DedicateRelay(_relays[i]));
            _relays[i].assigned_users += AssignedUserWeight::High;
        }
    }
    else
    {
        // TODO: Determine an effective way to return medium load level relays
    }

    return relays;
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
    // TODO: test if this can fail
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
    // Sending the request
    Response response = Communicator::SendRelayConnectionRequest(relay, JsonSerializer::SerializeRelayConnectionRequest());
    
    // Receiving the answer
    DedicatedRelay drel = JsonDeserializer::DeserializeRelayConnectionResponse(response);
    drel.ip = relay.ip;

    return drel;
}
