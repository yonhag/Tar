#pragma once
#include <stdint.h> // Used for int types

using PublicKey		= unsigned long;
using PrivateKey	= unsigned long;
using Totient		= unsigned long;
using Product		= unsigned long;
using Prime			= unsigned long;
using PossiblePrime = unsigned long;
using Integer		= unsigned long;

class RSA
{
public:
	RSA();
	RSA(const PublicKey& pubk, const PrivateKey& privk);


private:
	// Main methods
	void GeneratePrimes(Prime& P, Prime& Q) const; // P and Q are outputs
	Product CalculateProduct(const Prime P, const Prime Q) const;
	Totient CalculateTotient(const Prime P, const Prime Q) const;

	PublicKey SelectPublicKey(const Totient& t);
	PrivateKey SelectPrivateKey(const Totient& t, const PublicKey& pk);
	
	// Assistance methods
	static bool IsPrime(const PossiblePrime num);

	// Members
	PublicKey _PublicKey;
	PrivateKey _PrivateKey;
};
