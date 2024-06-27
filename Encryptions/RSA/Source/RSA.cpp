#include "RSA.h"
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <ctime>
#include <iostream>

RSA::RSA()
{
    /*
    Prime P = 0;
    Prime Q = 0;

    GeneratePrimes(P, Q);

    this->_product = CalculateProduct(P, Q);
    
    Totient t = CalculateTotient(P, Q);
    
    SelectPublicKey(t);
    SelectPrivateKey(t);
   */
    this->_product = 1485403;
    this->_PublicKey = 1358689;
    this->_PrivateKey = 837529;

    std::cout << this->_product << std::endl << this->_PublicKey << std::endl << this->_PrivateKey << std::endl;

}

RSA::RSA(const RSA& other) : _PublicKey(other._PublicKey), _PrivateKey(other._PrivateKey), _product(other._product)
{
}

RSA::RSA(const PublicKey& pubk, const PrivateKey& privk, const Product& product) : _PublicKey(pubk), _PrivateKey(privk), _product(product)
{
}

/*
    * SHOULD NOT BE USED EXCEPT FOR RSA HANDSHAKE,
*/
RSA::RSA(const PublicKey& pubk, const Product& product) : _PublicKey(pubk), _PrivateKey(0), _product(product)
{
}

Cipher RSA::Encrypt(const Plain& message) const
{
    return RSA::Encrypt(message, this->_PublicKey, this->_product);
}

Plain RSA::Decrypt(const Cipher& cipher) const
{
    Plain message;

    for (const Integer& val : cipher)
    {
        Integer n(val);
        n = powm(n, this->_PrivateKey, this->_product);
        message.push_back(n.convert_to<unsigned char>());
    }

    return message;
}

Cipher RSA::Encrypt(const Plain& message, const PublicKey& pkey, const Product& prod)
{
    Cipher cipher;

    for (const auto& uc : message)
    {
        Integer n(uc);
        n = powm(n, pkey, prod);
        cipher.push_back((n));
    }

    return cipher;
}

PublicKey RSA::GetPublicKey() const
{
    return this->_PublicKey;
}

PrivateKey RSA::GetPrivateKey() const
{
    return this->_PrivateKey;
}

Product RSA::GetProduct() const
{
    return this->_product;
}

void RSA::GeneratePrimes(Prime& P, Prime& Q) const
{
	// Preparing the RNG
    
	boost::random::mt19937 gen(std::time(0));
	boost::random::uniform_int_distribution<Prime> distribution(7, this->MAX_PRIMES);

    do 
    {
        P = distribution(gen);
    } while (!IsPrime(P));

    do
    {
        Q = distribution(gen);
    } while (!IsPrime(Q) || Q == P);
    // TODO: Add a test for if P and Q have a large difference between each other
}

Product RSA::CalculateProduct(const Prime& P, const Prime& Q) const
{
    return static_cast<Product>(P) * static_cast<Product>(Q);
}

Totient RSA::CalculateTotient(const Prime& P, const Prime& Q) const
{
    return (static_cast<Totient>(P) - 1) * (static_cast<Totient>(Q) - 1);
}

void RSA::SelectPublicKey(const Totient& t)
{
    boost::random::mt19937 gen(std::time(0));
    boost::random::uniform_int_distribution<PublicKey> distribution(7, t); // TODO: Integrate min_prime

    while (true)
    {
        this->_PublicKey = distribution(gen);
        if (CheckPublicKeyValidity(t))
            break;
    }
}

void RSA::SelectPrivateKey(Totient& totient)
{
    SignedInteger originalTotient = totient, temp, quotient;
    SignedInteger prevCoeff = 0, currCoeff = 1;
    SignedInteger publicKeyCopy = this->_PublicKey;

    while (publicKeyCopy > 1)
    {
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

bool RSA::IsPrime(const PossiblePrime& num)
{
    // No need to check 0/1, out of range for random function

    Integer sqrtNumber = sqrt(num);

    for (Integer i = 2; i <= sqrtNumber; i++) 
    {
        if (num % i == 0) 
            return false;
    }

    return true;
}

bool RSA::CheckPublicKeyValidity(const Totient& t) const
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

Plain RSA::CipherToPlain(const Cipher& input)
{
    Plain output;
    for (const auto& num : input) {
        std::vector<unsigned char> temp;
        export_bits(num, back_inserter(temp), 8);

        // Prefix each number with its byte length (assuming 4 bytes for the length prefix)
        size_t length = temp.size();
        for (int i = 3; i >= 0; --i) {
            output.push_back((length >> (i * 8)) & 0xFF);
        }

        // Append the serialized number
        output.insert(output.end(), temp.begin(), temp.end());
    }
    return output;
}

Cipher RSA::PlainToCipher(const Plain& input)
{
    Cipher output;
    size_t length = 0;

    for(size_t i = 0; i < input.size(); i += length)
    {
        // Read the length prefix (assuming 4 bytes for the length)
        length = 0;
        for (int j = 0; j < 4; ++j)
            length = (length << 8) | input[i++];

        // Read the bytes for the current number
        Integer num;
        import_bits(num, input.begin() + i, input.begin() + i + length, 8);
        output.push_back(num);

        i += length;
    }
    return output;
}
