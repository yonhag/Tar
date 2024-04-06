#include "RSA.h"
#include <vector>
#include <algorithm>

Plain CipherToPlain(const Cipher& input) 
{
    Plain output;
    for (const auto& num : input) {
        std::vector<unsigned char> temp;
        export_bits(num, back_inserter(temp), 8);

        // Prefix each number with its byte length (assuming 4 bytes for the length prefix)
        size_t length = temp.size();
        for (int i = 3; i >= 0; --i) {
            output.push_back((length >> (i * 8)) & 0xFF);
        }

        // Append the serialized number
        output.insert(output.end(), temp.begin(), temp.end());
    }
    return output;
}

Cipher PlainToCipher(const Plain& input) 
{
    Cipher output;
    size_t i = 0;
    while (i < input.size()) {
        // Read the length prefix (assuming 4 bytes for the length)
        size_t length = 0;
        for (int j = 0; j < 4; ++j) {
            length = (length << 8) | input[i++];
        }

        // Read the bytes for the current number
        cpp_int num;
        import_bits(num, input.begin() + i, input.begin() + i + length, 8);
        output.push_back(num);

        i += length;
    }
    return output;
}

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

    std::vector<unsigned char> y = CipherToPlain(x);
    x = rsa.Encrypt(y);
    for (auto& i : x)
    {
        std::cout << i;
        std::cout << " ";
    }
    std::cout << std::endl;
    y = rsa.Decrypt(x);
    x = PlainToCipher(y);
    for (auto& i : x)
    {
        std::cout << i;
        std::cout << " ";
    }
    std::cout << std::endl;


    return 0;
}
