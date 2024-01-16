#include "RSA.h"
#include <iostream>
int main()
{
	RSA cipher;
	std::vector<unsigned char> vec;
	vec.push_back('1');
	std::cout << "Vector has: " << vec[0] << std::endl;
	vec = cipher.Encrypt(vec);
	std::cout << "After Encryption: " << vec[0] << std::endl;
	vec = cipher.Decrypt(vec);
	std::cout << "After Decryption: " << vec[0] << std::endl;

	return 0;
}