#include "AES.h"
#include <iostream>
#include <string>
#include <vector>

int main() 
{
	std::vector<unsigned char> plain = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0xcc}; //plaintext example
	std::vector<unsigned char> key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; //key example

	AES aes(AESKeyLength::AES_128);
	std::vector<unsigned char> c = aes.EncryptCBC(plain);
	aes.printHexVector(plain);
	std::cout << std::endl;
	std::cout << std::endl;
	aes.printHexVector(c);
	std::cout << std::endl;
	std::cout << std::endl;
	std::vector<unsigned char> d = aes.DecryptCBC(c);
	aes.printHexVector(d);
	std::cout << std::endl;
	std::cout << std::endl;
	return 0;
}