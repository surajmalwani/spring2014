#include<stdint.h>
#include<errorcodes.h>

#ifndef ENCRYPT_H_INCLUDED
#define ENCRYPT_H_INCLUDED
#endif

//Encryption Constants
#define SECURE_MEMORY_SIZE_IN_BYTES 16384
#define KEY_ALGO GCRY_KDF_PBKDF2
#define KEY_HASH_ALGO GCRY_MD_SHA512
#define NUMBER_OF_ITERATIONS 4096
#define KEY_SALT "NaCl"
#define KEY_SIZE 32
#define ENCRYPTION_ALGO GCRY_CIPHER_AES128
#define ENCRYPTION_MODE GCRY_CIPHER_MODE_CBC
#define INIT_VECTOR 5844



void encrypt_file_inpath(char* filepath, const char* password);
void encrypt_file(FILE* file, const char* password);
void generate_key(const char* password,  uint8_t keybuffer[]);
char* readFileIntoString(char* filepath);