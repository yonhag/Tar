#include "RSA.h"
#include <vector>
#include <algorithm>

int main() {
    // Example usage
    RSA rsa;
    std::vector<Integer> x = { rsa.GetPrivateKey(), rsa.GetPublicKey()};
    for (auto& i : x)
    {
        std::cout << i;
        std::cout << " ";
    }
    std::cout << std::endl;

    std::vector<unsigned char> y = RSA::CipherToPlain(x);
    x = rsa.Encrypt(y); 
    for (auto& i : x)
    {
        std::cout << i;
        std::cout << " ";
    }
    std::cout << std::endl;
    y = rsa.Decrypt(x);
    x = RSA::PlainToCipher(y);
    
    for (auto& i : x)
    {
        std::cout << i;
        std::cout << " ";
    }
    std::cout << std::endl;

    return 0;
}
