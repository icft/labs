#ifndef KUZNECHIK_H
#define KUZNECHIK_H

#include <stdint.h>

/*!
*       \brief Структура для шифрования
*/
typedef union {
    uint64_t q[2];
    uint8_t  b[16];
} w128_t;


/*!
*       \brief Структура для шифрования
*/
typedef struct {
    w128_t k[10];		
} kuz_key_t;

void kuz_init();

void kuz_set_encrypt_key(kuz_key_t* subkeys, const uint8_t key[32]);
void kuz_set_decrypt_key(kuz_key_t* subkeys, const uint8_t key[32]);

void kuz_encrypt_block(kuz_key_t* subkeys, void* x);
void kuz_decrypt_block(kuz_key_t* subkeys, void* x);

char* kuz_encrypt(kuz_key_t* subkeys, char* from, uint32_t* len);
char* kuz_decrypt(kuz_key_t* subkeys, char* from, uint32_t len);
char* kuz_encrypt_ctr(kuz_key_t* subkeys, char* from, uint32_t len);
#endif
