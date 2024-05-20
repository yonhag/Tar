#include "RSA.h"

int main()
{
	RSA rsa;

	std::vector<unsigned char> vec = { 1, 2, 3, 15, 255, 123 };

	for (auto& i : vec)
		std::cout << i;
	std::cout << std::endl;
	auto x = rsa.Encrypt(vec);
	

	for (auto& i : x)
		std::cout << i;
	std::cout << std::endl;

	vec = rsa.Decrypt(x);


	for (auto& i : vec)
		std::cout << i;
	std::cout << std::endl;

	return 0;
}