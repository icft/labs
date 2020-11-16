#include "Aes256.h"

uint8_t getSBoxValue(uint8_t num)
{
    return uint8_t();
}

uint8_t getSBoxInvert(uint8_t num)
{
    return uint8_t();
}

void KeyExpansion(uint8_t* RoundKey, const uint8_t* Key)
{
}

void AES_init_ctx(AES_ctx* ctx, const uint8_t* key)
{
}

void AddRoundKey(uint8_t round, state_t* state, uint8_t* RoundKey)
{
}

void SubBytes(state_t* state)
{
}

void ShiftRows(state_t* state)
{
}

uint8_t xtime(uint8_t x)
{
    return uint8_t();
}

void MixColumns(state_t* state)
{
}

uint8_t Multiply(uint8_t x, uint8_t y)
{
    return uint8_t();
}

void InvMixColumns(state_t* state)
{
}

void InvSubBytes(state_t* state)
{
}

void InvShiftRows(state_t* state)
{
}

void Cipher(state_t* state, uint8_t* RoundKey)
{
}

void InvCipher(state_t* state, uint8_t* RoundKey)
{
}

void AES_ECB_encrypt(AES_ctx* ctx, uint8_t* buf)
{
}

void AES_ECB_decrypt(AES_ctx* ctx, uint8_t* buf)
{
}

void AES_encrypt(const uint8_t* key, uint8_t* data, uint32_t size)
{
}

void AES_decrypt(const uint8_t* key, uint8_t* data, uint32_t size)
{
}

void PrintHex(const uint8_t* str, uint8_t len)
{
}

int ctoh(char c)
{
    return 0;
}

uint8_t chartohex(char c)
{
    return uint8_t();
}
