#ifndef _AES_H_
#define _AES_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

enum keySize
{
    SIZE_16 = 16,
    SIZE_24 = 24,
    SIZE_32 = 32
};

enum Error
{
    SUCCESS = 0,
    AES_KEYSIZE_MISMATCH,
    MEMORY_ALLOCATION_FAILED,
};

extern const int EXPANDED_KEY_SIZE;

// Implementation: S-Box
extern uint8_t SBOX[256];
// Implementation: Inverse S-Box
extern uint8_t INV_SBOX[256];
// Implementation: RCON
extern uint8_t RCON[255];


// Functions to get the value of Sbox index and invSbox index
uint8_t getSBox(uint8_t num);
uint8_t getSBoxInv(uint8_t num);

// Implementation: Rotate
void rotate(uint8_t *word);

// Impl : get value from RCON
uint8_t getRCON(uint8_t num);

// Key Schedule Core
void core(uint8_t *word, int iteration);

// Key Expansion
void expandKey(uint8_t *expandedKey, uint8_t *key, enum keySize, size_t expandedKeySize);

// Supporting functions for AES Encryption

// sub_bytes
void sub_bytes(uint8_t *state);

// shift_rows
void shift_rows(uint8_t *state);
void shift_row(uint8_t *state, uint8_t r);

// Multiplication under Galois field GF(256)
uint8_t gf_mul(uint8_t a, uint8_t b);

// mix_columns
void mix_columns(uint8_t *state);
void mix_column(uint8_t *col);

// Implement a round in AES
void aes_round(uint8_t *state, uint8_t *round_key);

// The main AES body
void create_round_key(uint8_t *expanded_key, uint8_t *round_key);
void aes_main(uint8_t *state, uint8_t *expanded_key, int no_round);

// The main function for encryption
char aes_encrypt_user(uint8_t *pt, uint8_t *ct, uint8_t *key, enum keySize size);

// Supporting function for AES decryption

// sub_bytes
void inv_sub_bytes(uint8_t *state);

// shift_rows
void inv_shift_rows(uint8_t *state);
void inv_shift_row(uint8_t *state, uint8_t r);

// mix_columns
void inv_mix_columns(uint8_t *state);
void inv_mix_column(uint8_t *column);

// Implementation for an AES decryption rounf
void aes_inv_round(uint8_t *state, uint8_t *round_key);

// Main AES body
void aes_inv_main(uint8_t *state, uint8_t *expanded_key, int no_round);

// Main function for AES decryption
char aes_decrypt_user(uint8_t *ct, uint8_t *pt, uint8_t *key, enum keySize size);

// Implementation: addRoundKey
void add_round_key(uint8_t *state, uint8_t *round_key);

// Some misc functions
void rand_init();


// Random function and display function
uint8_t * random_block(int block_len);
void print_block(uint8_t *block, int block_len);

// Padding function, return data_len + pad_ammount = multiple of 16
int calc_pad_ammount(int data_len);

// Padding functions and misc functions
uint8_t * pad(uint8_t *block, int block_len);
uint8_t * unpad(uint8_t *data, int data_len);
int length_after_unpad(uint8_t *data_pad, int data_len);
int length_after_pad(int data_len);

// AES-CBC using AES128 with an additional Initialization Vector (IV)
uint8_t * aes_cbc_encrypt(uint8_t *pt, int pt_len, uint8_t *key, uint8_t *iv);
uint8_t * aes_cbc_decrypt(uint8_t *ct, int ct_len, uint8_t *key, uint8_t *iv);

// Test functions
void test_aes(int round);
void test_aes_cbc(int round, int plaintext_len);
void test_pad_and_unpad(int round);
bool check_two_arrays(uint8_t *a1, uint8_t *a2, int l);

void example_aes128_cbc(uint8_t* plaintext, int plaintext_len);
void example_encrypt_to_python_file();

#endif
