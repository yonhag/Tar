#pragma once

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

	void SelectPublicKey(const Totient t);
	void SelectPrivateKey(const Totient t);
	
	// Assistance methods
	static bool IsPrime(const PossiblePrime num);
	bool CheckPublicKeyValidity(const Totient t) const;
	bool CheckPrivateKeyValidity(const Totient t) const;

	// Members
	// Keys
		PublicKey _PublicKey;
		PrivateKey _PrivateKey;
	// Additionals
		Product _product;
};
