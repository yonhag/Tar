#include "AES.h"
#include <random>
#include <chrono>

AES::AES(const AESKeyLength keyLength) {
    int key_length = 0;
    switch (keyLength) {
    case AESKeyLength::AES_128:
        this->Nk = 4;
        this->Nr = 10;
        key_length = 16;
        break;
    case AESKeyLength::AES_192:
        this->Nk = 6;
        this->Nr = 12;
        key_length = 24;
        break;
    case AESKeyLength::AES_256:
        this->Nk = 8;
        this->Nr = 14;
        key_length = 32;
        break;
    }
    generateRandomBytes(key, key_length);
    generateRandomBytes(iv, key_length);
}

AES::AES(unsigned char key[], unsigned char iv[], const AESKeyLength keyLength)
{
    switch (keyLength) {
    case AESKeyLength::AES_128:
        this->Nk = 4;
        this->Nr = 10;
        break;
    case AESKeyLength::AES_192:
        this->Nk = 6;
        this->Nr = 12;
        break;
    case AESKeyLength::AES_256:
        this->Nk = 8;
        this->Nr = 14;
        break;
    }
    this->key = key;
    this->iv = iv;
}

void AES::generateRandomBytes(unsigned char* buffer, int length)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (int i = 0; i < length; ++i) {
        buffer[i] = static_cast<unsigned char>(dis(gen));
    }
}

unsigned char* AES::get_key()
{
    return key;
}

unsigned char* AES::get_iv()
{
    return iv;
}

unsigned char* AES::EncryptCBC(unsigned char in[], unsigned int inLen)
{
    padToMultipleOf16(in, inLen);
    //CheckLength(inLen);
    unsigned char* out = new unsigned char[inLen];
    unsigned char block[blockBytesLen];
    unsigned char* roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
    KeyExpansion(key, roundKeys);
    memcpy(block, iv, blockBytesLen);
    for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
        XorBlocks(block, in + i, block, blockBytesLen);
        EncryptBlock(block, out + i, roundKeys);
        memcpy(block, out + i, blockBytesLen);
    }

    delete[] roundKeys;

    return out;
}

unsigned char* AES::DecryptCBC(unsigned char in[], unsigned int inLen) 
{
    //CheckLength(inLen);
    unsigned char* out = new unsigned char[inLen];
    unsigned char block[blockBytesLen];
    unsigned char* roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
    KeyExpansion(key, roundKeys);
    memcpy(block, iv, blockBytesLen);
    for (unsigned int i = 0; i < inLen; i += blockBytesLen) {
        DecryptBlock(in + i, out + i, roundKeys);
        XorBlocks(block, out + i, out + i, blockBytesLen);
        memcpy(block, in + i, blockBytesLen);
    }

    delete[] roundKeys;
    removePadding(out, inLen);
    return out;
}

void AES::CheckLength(unsigned int len) {
    if (len % blockBytesLen != 0) 
    {
        throw std::length_error("Plaintext length must be divisible by " +
            std::to_string(blockBytesLen));
    }
}

void AES::EncryptBlock(const unsigned char in[], unsigned char out[],
    unsigned char* roundKeys) {
    unsigned char state[4][Nb];
    unsigned int i, j, round;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < Nb; j++) {
            state[i][j] = in[i + 4 * j];
        }
    }

    AddRoundKey(state, roundKeys);

    for (round = 1; round <= Nr - 1; round++) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, roundKeys + round * 4 * Nb);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, roundKeys + Nr * 4 * Nb);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < Nb; j++) {
            out[i + 4 * j] = state[i][j];
        }
    }
}

void AES::padToMultipleOf16(unsigned char* array, unsigned int length)
{
    int remainder = length % 16;
    int paddingNeeded = remainder == 0 ? 0 : (16 - remainder);

    if (paddingNeeded > 0) {
        unsigned char paddingValue = static_cast<unsigned char>(paddingNeeded);
        unsigned char* newArray = new unsigned char[length + paddingNeeded];
        memcpy(newArray, array, length);
        for (int i = length; i < length + paddingNeeded; ++i) {
            newArray[i] = paddingValue;
        }
        delete[] array;
        array = newArray;
        length += paddingNeeded;
    }
}

void AES::removePadding(unsigned char* array, unsigned int length)
{
    if (length == 0) {
        return;
    }

    int paddingLength = array[length - 1];
    if (paddingLength > length) {
        // Invalid padding length, do nothing
        return;
    }

    // Check if the padding bytes are consistent
    for (int i = length - paddingLength; i < length; ++i) {
        if (array[i] != paddingLength) {
            // Invalid padding bytes, do nothing
            return;
        }
    }

    // Remove padding bytes
    length -= paddingLength;
}

void AES::DecryptBlock(const unsigned char in[], unsigned char out[],
    unsigned char* roundKeys) {
    unsigned char state[4][Nb];
    unsigned int i, j, round;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < Nb; j++) {
            state[i][j] = in[i + 4 * j];
        }
    }

    AddRoundKey(state, roundKeys + Nr * 4 * Nb);

    for (round = Nr - 1; round >= 1; round--) {
        InvSubBytes(state);
        InvShiftRows(state);
        AddRoundKey(state, roundKeys + round * 4 * Nb);
        InvMixColumns(state);
    }

    InvSubBytes(state);
    InvShiftRows(state);
    AddRoundKey(state, roundKeys);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < Nb; j++) {
            out[i + 4 * j] = state[i][j];
        }
    }
}

void AES::SubBytes(unsigned char state[4][Nb]) {
    unsigned int i, j;
    unsigned char t;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < Nb; j++) {
            t = state[i][j];
            state[i][j] = sbox[t / 16][t % 16];
        }
    }
}

void AES::ShiftRow(unsigned char state[4][Nb], unsigned int i,
    unsigned int n)  // shift row i on n positions
{
    unsigned char tmp[Nb];
    for (unsigned int j = 0; j < Nb; j++) {
        tmp[j] = state[i][(j + n) % Nb];
    }
    memcpy(state[i], tmp, Nb * sizeof(unsigned char));
}

void AES::ShiftRows(unsigned char state[4][Nb]) {
    ShiftRow(state, 1, 1);
    ShiftRow(state, 2, 2);
    ShiftRow(state, 3, 3);
}

unsigned char AES::xtime(unsigned char b)  // multiply on x
{
    return (b << 1) ^ (((b >> 7) & 1) * 0x1b);
}

void AES::MixColumns(unsigned char state[4][Nb]) {
    unsigned char temp_state[4][Nb];

    for (size_t i = 0; i < 4; ++i) {
        memset(temp_state[i], 0, 4);
    }

    for (size_t i = 0; i < 4; ++i) {
        for (size_t k = 0; k < 4; ++k) {
            for (size_t j = 0; j < 4; ++j) {
                if (CMDS[i][k] == 1)
                    temp_state[i][j] ^= state[k][j];
                else
                    temp_state[i][j] ^= GF_MUL_TABLE[CMDS[i][k]][state[k][j]];
            }
        }
    }

    for (size_t i = 0; i < 4; ++i) {
        memcpy(state[i], temp_state[i], 4);
    }
}

void AES::AddRoundKey(unsigned char state[4][Nb], unsigned char* key) {
    unsigned int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < Nb; j++) {
            state[i][j] = state[i][j] ^ key[i + 4 * j];
        }
    }
}

void AES::SubWord(unsigned char* a) {
    int i;
    for (i = 0; i < 4; i++) {
        a[i] = sbox[a[i] / 16][a[i] % 16];
    }
}

void AES::RotWord(unsigned char* a) {
    unsigned char c = a[0];
    a[0] = a[1];
    a[1] = a[2];
    a[2] = a[3];
    a[3] = c;
}

void AES::XorWords(unsigned char* a, unsigned char* b, unsigned char* c) {
    int i;
    for (i = 0; i < 4; i++) {
        c[i] = a[i] ^ b[i];
    }
}

void AES::Rcon(unsigned char* a, unsigned int n) {
    unsigned int i;
    unsigned char c = 1;
    for (i = 0; i < n - 1; i++) {
        c = xtime(c);
    }

    a[0] = c;
    a[1] = a[2] = a[3] = 0;
}

void AES::KeyExpansion(const unsigned char key[], unsigned char w[]) {
    unsigned char temp[4];
    unsigned char rcon[4];

    unsigned int i = 0;
    while (i < 4 * Nk) {
        w[i] = key[i];
        i++;
    }

    i = 4 * Nk;
    while (i < 4 * Nb * (Nr + 1)) {
        temp[0] = w[i - 4 + 0];
        temp[1] = w[i - 4 + 1];
        temp[2] = w[i - 4 + 2];
        temp[3] = w[i - 4 + 3];

        if (i / 4 % Nk == 0) {
            RotWord(temp);
            SubWord(temp);
            Rcon(rcon, i / (Nk * 4));
            XorWords(temp, rcon, temp);
        }
        else if (Nk > 6 && i / 4 % Nk == 4) {
            SubWord(temp);
        }

        w[i + 0] = w[i - 4 * Nk] ^ temp[0];
        w[i + 1] = w[i + 1 - 4 * Nk] ^ temp[1];
        w[i + 2] = w[i + 2 - 4 * Nk] ^ temp[2];
        w[i + 3] = w[i + 3 - 4 * Nk] ^ temp[3];
        i += 4;
    }
}

void AES::InvSubBytes(unsigned char state[4][Nb]) {
    unsigned int i, j;
    unsigned char t;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < Nb; j++) {
            t = state[i][j];
            state[i][j] = inv_sbox[t / 16][t % 16];
        }
    }
}

void AES::InvMixColumns(unsigned char state[4][Nb]) {
    unsigned char temp_state[4][Nb];

    for (size_t i = 0; i < 4; ++i) {
        memset(temp_state[i], 0, 4);
    }

    for (size_t i = 0; i < 4; ++i) {
        for (size_t k = 0; k < 4; ++k) {
            for (size_t j = 0; j < 4; ++j) {
                temp_state[i][j] ^= GF_MUL_TABLE[INV_CMDS[i][k]][state[k][j]];
            }
        }
    }

    for (size_t i = 0; i < 4; ++i) {
        memcpy(state[i], temp_state[i], 4);
    }
}

void AES::InvShiftRows(unsigned char state[4][Nb]) {
    ShiftRow(state, 1, Nb - 1);
    ShiftRow(state, 2, Nb - 2);
    ShiftRow(state, 3, Nb - 3);
}

void AES::XorBlocks(const unsigned char* a, const unsigned char* b,
    unsigned char* c, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        c[i] = a[i] ^ b[i];
    }
}

void AES::printHexArray(unsigned char a[], unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        printf("%02x ", a[i]);
    }
}

void AES::printHexVector(std::vector<unsigned char> a) {
    for (unsigned int i = 0; i < a.size(); i++) {
        printf("%02x ", a[i]);
    }
}

std::vector<unsigned char> AES::ArrayToVector(unsigned char* a,
    unsigned int len) {
    std::vector<unsigned char> v(a, a + len * sizeof(unsigned char));
    return v;
}

unsigned char* AES::VectorToArray(std::vector<unsigned char>& a) {
    return a.data();
}

std::vector<unsigned char> AES::EncryptCBC(std::vector<unsigned char> in) 
{
    unsigned char* out = EncryptCBC(VectorToArray(in), (unsigned int)in.size());
    std::vector<unsigned char> v = ArrayToVector(out, in.size());
    delete[] out;
    return v;
}

std::vector<unsigned char> AES::DecryptCBC(std::vector<unsigned char> in) 
{
    unsigned char* out = DecryptCBC(VectorToArray(in), (unsigned int)in.size());
    std::vector<unsigned char> v = ArrayToVector(out, (unsigned int)in.size());
    delete[] out;
    return v;
}