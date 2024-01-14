#include "RSA.h"
#include <random> // Used for randomizing primes

void RSA::GeneratePrimes(Prime& P, Prime& Q) const
{
    const unsigned long min_prime = 10000000;
	// Preparing the RNG
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(7, ULLONG_MAX); // TODO: Integrate min_prime, ULLONG / ULONG

    do 
    {
        P = distribution(gen);
    } while (!IsPrime(P));

    do
    {
        Q = distribution(gen);
    } while (!IsPrime(Q));
    // TODO: Add a test for if P and Q have a large difference between each other
}

Product RSA::CalculateProduct(const Prime P, const Prime Q) const
{
    return P * Q;
}

Totient RSA::CalculateTotient(const Prime P, const Prime Q) const
{
    return (P - 1) * (Q - 1);
}

PublicKey RSA::SelectPublicKey(const Totient t)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(7, ULLONG_MAX); // TODO: Integrate min_prime, ULLONG / ULONG

    while (true)
    {
        this->_PublicKey = distribution(gen);
        if (CheckPublicKeyValidity(t))
            break;
    }
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

bool RSA::CheckPublicKeyValidity(const Totient t) const
{
    // If larger than totient
    if (!(t < this->_PublicKey))
        return false;
    // If not prime itself
    if (!IsPrime(this->_PublicKey))
        return false;
    // If key is a factor of totient
    if (t % this->_PublicKey == 0)
        return false;
    return true;
}
