#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <vector>

using namespace boost::multiprecision;

using PublicKey = cpp_int;
using PrivateKey = cpp_int;
using Totient = cpp_int;
using Product = cpp_int;
using Prime = cpp_int;
using PossiblePrime = cpp_int;
using Integer = cpp_int;
using SignedInteger = cpp_int;

using Cipher = std::vector<Integer>;
using Plain = std::vector<unsigned char>;

class RSA
{
public:
	RSA();
	RSA(const RSA& other) = default;
	RSA(const PublicKey& pubk, const PrivateKey& privk, const Product& product);
	RSA(const PublicKey& pubk, const Product& product);

	Cipher Encrypt(const Plain& message) const;
	Plain Decrypt(const Cipher& cipher) const;

	static Cipher Encrypt(const Plain& message, const PublicKey& pkey, const Product& prod);

	PublicKey GetPublicKey() const;
	PrivateKey GetPrivateKey() const;
	Product GetProduct() const;

	// Convertion methods
	static Plain CipherToPlain(const Cipher& input);
	static Cipher PlainToCipher(const Plain& input);
private:
	// Main methods
	void GeneratePrimes(Prime& P, Prime& Q) const; // P and Q are outputs
	Product CalculateProduct(const Prime& P, const Prime& Q) const;
	Totient CalculateTotient(const Prime& P, const Prime& Q) const;

	void SelectPublicKey(const Totient& t);
	void SelectPrivateKey(Totient& t);

	// Assistance methods
	static bool IsPrime(const PossiblePrime& num);
	bool CheckPublicKeyValidity(const Totient& t) const;

	// Members
	// Keys
	PublicKey _PublicKey;
	PrivateKey _PrivateKey;
	// Additionals
	Product _product;

	static constexpr int MAX_PRIMES = 10000;
};
