#include<stdint.h>

#ifndef CRYPTCOMMON_H_INCLUDED
#define CRYPTCOMMON_H_INCLUDED
#endif

#define SECURE_MEMORY_SIZE_IN_BYTES 16384
#define KEY_ALGO GCRY_KDF_PBKDF2
#define KEY_HASH_ALGO GCRY_MD_SHA512
#define NUMBER_OF_ITERATIONS 4096
#define KEY_SALT "NaCl"
#define KEY_SIZE 16
#define MAC_SIZE 64
#define ENCRYPTION_ALGO GCRY_CIPHER_AES128
#define ENCRYPTION_MODE GCRY_CIPHER_MODE_CBC
#define PASSWORD_SIZE 15

static int IV[] = {5,8,4,4};

void init_gcrypt();
void generate_key(char* password,  uint8_t keybuffer[]);
char* generateHMAC(char* inputString, uint8_t keybuffer[]);
void finalize_gcrypt();
char* requestPassphrase();
static long getFileEncryptedLength(long fileLength);



