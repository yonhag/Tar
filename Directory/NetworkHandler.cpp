#include "NetworkHandler.h"
#include <random>
#include <algorithm>

std::vector<Relay> NetworkHandler::GetRelays()
{
    // Making sure there are enough relays avilable
    if (this->_relays.size() < 3)
        return std::vector<Relay>();

    std::vector<Relay> relays;
    std::vector<int> used_relays;

    // Setting the seed for randomizer
    std::random_device rd;
    std::mt19937 mt{ rd() };

    // Setting the distribution - which is the this->_relays index range
    std::uniform_int_distribution<int> dist{ 0, this->_relays.size() - 1 };

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
        relays.push_back(this->_relays[current_index]);
    }
}
