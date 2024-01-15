#pragma once
#include <vector>

using PublicKey		= unsigned long long;
using PrivateKey	= unsigned long long;
using Totient		= unsigned long long;
using Product		= unsigned long long;
using Prime			= unsigned long;
using PossiblePrime = unsigned long long;
using Integer		= unsigned long;

class RSA
{
public:
	RSA();
	RSA(const PublicKey& pubk, const PrivateKey& privk);

	std::vector<unsigned char> Encrypt(const std::vector<unsigned char>& message);
	std::vector<unsigned char> Decrypt(const std::vector<unsigned char>& cipher);

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
