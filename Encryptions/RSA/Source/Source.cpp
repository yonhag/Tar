#include "RSA.h"

int main()
{
	std::string og = "\"JSON\": \"Balls\"";

	Plain vec(og.begin(), og.end());

	for (const auto& i : vec)
		std::cout << i;
	std::cout << std::endl;

	RSA rsa;
	auto x = rsa.Encrypt(vec);

	for (const auto& i : x)
		std::cout << i;
	std::cout << std::endl;

	auto y = RSA::CipherToPlain(x);
	for (const auto& i : y)
		std::cout << i;
	std::cout << std::endl;
	x = RSA::PlainToCipher(y);

	for (const auto& i : x)
		std::cout << i;
	std::cout << std::endl;

	y = rsa.Decrypt(x);

	for (auto& i : y)
		std::cout << i;
	std::cout << std::endl;

	return 0;
}