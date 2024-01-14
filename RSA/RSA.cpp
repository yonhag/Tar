#include "RSA.h"
#include <random> // Used for randomizing primes

void RSA::GeneratePrimeNumbers(Prime& P, Prime& Q)
{
	// Preparing the RNG
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(7, sizeof(Integer));



}

bool RSA::IsPrime(const PossiblePrime num)
{
    // No need to check 0/1, out of range for random function

    auto sqrtNumber = static_cast<Integer>(std::sqrt(num));

    for (Integer i = 2; i <= sqrtNumber; i++) 
    {
        if (num % i == 0) 
            return false;
    }

    return true;
}
