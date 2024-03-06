#include "RSA.h"
#include <thread>
#include <iostream>

template <typename T>
void PrintVector(const std::vector<T>& vec);

int main()
{
	int trues = 0;
	for (int i = 0; i < 10; i++)
	{
		RSA cipher;
		std::vector<unsigned char> vec;
		vec.push_back('1');
		vec.push_back('2');
		vec.push_back('3');

		PrintVector<unsigned char>(vec);

		Cipher tor = cipher.Encrypt(vec);
		PrintVector<Integer>(tor);

		vec = cipher.Decrypt(tor);
		PrintVector<unsigned char>(vec);

		//trues += CompareVector(vec, tor);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << std::endl;
	}
	std::cout << trues;

	return 0;
}

template <typename T>
void PrintVector(const std::vector<T>& vec)
{
	for (auto& i : vec)
		std::cout << static_cast<int>(i) << ' ';
	std::cout << std::endl;
}
