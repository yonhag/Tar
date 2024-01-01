#include "NetworkManager.h"
#include "Response.h"
#include "Communicator.h"
#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include <random>
#include <algorithm>

// Initializing the relay vector
// Value can be changed for testing
std::vector<Relay> NetworkManager::_relays;

std::vector<DedicatedRelay> NetworkManager::GetRelays(const LoadLevel loadlevel)
{
    // Making sure there are enough relays avilable
    if (_relays.size() < 3)
        return std::vector<DedicatedRelay>();

    std::vector<DedicatedRelay> relays;
    std::vector<int> used_relays;

    // Setting the seed for randomizer
    std::random_device rd;
    std::mt19937 mt{ rd() };

    // Setting the distribution - which is the _relays index range
    std::uniform_int_distribution<int> dist{ 0, (static_cast<int>(_relays.size()) - 1) }; // Static cast is fine since number shouldn't be too high

    for (int i = 0; i < relays_per_user; i++)
    {
        int current_index;

        do {
            // Generating the number
            current_index = dist(mt);

            // Making sure it hasn't been used yet
            auto it = std::find(used_relays.begin(), used_relays.end(), current_index);
            if (it == used_relays.end())
                break;
        } while (true); // While (true) to avoid declaring {it} before the loop

        // Pushing back a number
        relays.push_back(DedicateRelay(_relays[current_index]));
    }

    return relays;
}

void NetworkManager::JoinNetwork(const std::string& ip, const unsigned int bandwidth)
{
    Relay newRelay;
    newRelay.ip = ip;
    newRelay.bandwidth = bandwidth;

    _relays.push_back(newRelay);
    Communicator::
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
