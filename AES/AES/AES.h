#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#define NUM_ROUND_KEYS_128 (11)

typedef uint8_t AES_Column_t[4];
typedef AES_Column_t AES_Block_t[4];
typedef uint8_t AES_Key128_t[16];

uint8_t GF_Mult(uint8_t a, uint8_t b);
void GF_WordAdd(AES_Column_t a, AES_Column_t b, AES_Column_t dest);

void AES_KeySchedule128(const AES_Key128_t key, AES_Block_t* keysOut);

void AES_RotWord(AES_Column_t word);
void AES_SubBytes(AES_Block_t state, const uint8_t table[]);
void AES_SubWord(AES_Column_t word, const uint8_t table[]);
void AES_ShiftRows(AES_Block_t state);
void AES_AddRoundKey(AES_Block_t state, const AES_Block_t roundKey);
void AES_MixColumns(AES_Block_t state);