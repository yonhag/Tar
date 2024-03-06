#pragma once
#include "boost_1_84_0/boost/multiprecision/cpp_int.hpp"
#include <vector>

using namespace boost::multiprecision;

using PublicKey		= cpp_int;
using PrivateKey	= cpp_int;
using Totient		= cpp_int;
using Product		= cpp_int;
using Prime			= cpp_int;
using PossiblePrime = cpp_int;
using Integer		= cpp_int;
using SignedInteger = cpp_int;

using Cipher = std::vector<Integer>;
using Plain = std::vector<unsigned char>;

class RSA
{
public:
	RSA();
	RSA(const PublicKey& pubk, const PrivateKey& privk);

	Cipher Encrypt(const Plain& message) const;
	Plain Decrypt(const Cipher& cipher) const;

private:
	// Main methods
	void GeneratePrimes(Prime& P, Prime& Q) const; // P and Q are outputs
	Product CalculateProduct(const Prime& P, const Prime& Q) const;
	Totient CalculateTotient(const Prime& P, const Prime& Q) const;

	void SelectPublicKey(const Totient t);
	void SelectPrivateKey(const Totient t);
	
	// Assistance methods
	static bool IsPrime(const PossiblePrime num);
	bool CheckPublicKeyValidity(const Totient t) const;

	// Members
	// Keys
		PublicKey _PublicKey;
		PrivateKey _PrivateKey;
	// Additionals
		Product _product;

		const int MAX_PRIMES = 10000;
};
