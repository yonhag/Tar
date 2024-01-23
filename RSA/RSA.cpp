#include "RSA.h"
#include <random> // Used for randomizing primes
#include <iostream>

RSA::RSA()
{
    Prime P = 0;
    Prime Q = 0;

    GeneratePrimes(P, Q);

    this->_product = CalculateProduct(P, Q);

    Totient t = CalculateTotient(P, Q);
    
    SelectPublicKey(t);
    SelectPrivateKey(t);
    std::cout << "Public: " << this->_PublicKey << std::endl;
    std::cout << "Private: " << this->_PrivateKey << std::endl;
    std::cout << "Totient: " << t << std::endl;
    std::cout << "Product: " << this->_product << std::endl;
}

std::vector<unsigned char> RSA::Encrypt(const std::vector<unsigned char>& message) const
{
    std::vector<unsigned char> cipher;
    
    for (const auto& uc : message)
    {
        Integer n = uc;
        n = (n ^ this->_PublicKey) % this->_product;
        cipher.push_back(n);
    }

    return cipher;
}

std::vector<unsigned char> RSA::Decrypt(const std::vector<unsigned char>& cipher) const
{
    std::vector<unsigned char> message;

    for (const auto& val : cipher)
    {
        Integer n = val;
        n = (n ^ this->_PrivateKey) % this->_product;
        message.push_back(n);
    }

    return message;
}

void RSA::GeneratePrimes(Prime& P, Prime& Q) const
{
	// Preparing the RNG
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<Prime> distribution(7, this->MAX_PRIMES);

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
    return static_cast<Product>(P) * static_cast<Product>(Q);
}

Totient RSA::CalculateTotient(const Prime P, const Prime Q) const
{
    return (static_cast<Totient>(P) - 1) * (static_cast<Totient>(Q) - 1);
}

void RSA::SelectPublicKey(const Totient t)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<PublicKey> distribution(7, t); // TODO: Integrate min_prime

    while (true)
    {
        this->_PublicKey = distribution(gen);
        if (CheckPublicKeyValidity(t))
            break;
    }
}

void RSA::SelectPrivateKey(Totient totient)
{
        SignedInteger originalTotient = totient, temp, quotient;
        SignedInteger prevCoeff = 0, currCoeff = 1;

        if (totient == 1) {
            this->_PrivateKey = 0;
            return;
        }

        SignedInteger publicKeyCopy = this->_PublicKey;
        while (publicKeyCopy > 1) {
            quotient = publicKeyCopy / totient;
            temp = totient;
            totient = publicKeyCopy % totient;
            publicKeyCopy = temp;
            temp = prevCoeff;
            prevCoeff = currCoeff - quotient * prevCoeff;
            currCoeff = temp;
        }

        if (currCoeff < 0)
            currCoeff += originalTotient;

        this->_PrivateKey = currCoeff;
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
    // Guarenteed to be larger than totient, since generation
    // If key is a factor of totient
    if (t % this->_PublicKey == 0)
        return false;
    // If not prime itself
    if (!IsPrime(this->_PublicKey))
        return false;
    return true;
}
