/*
 * verify_v2.c — host-side cross-check for the 12 new algorithms added
 * in the second expansion (SHA-3-512, PBKDF2-HMAC-SHA256, AES-CBC-MAC,
 * CRC-64, Fletcher-16/32/64, Knuth / OAT / PJW / SDBM, Murmur3-128).
 *
 * Build:
 *   gcc -O2 -I../include -o verify_v2 verify_v2.c \
 *       ../source/tiny_hashes.c ../source/fletcher.c ../source/crc64.c \
 *       ../source/murmur3_128.c ../source/sha3.c ../source/pbkdf2_sha256.c \
 *       ../source/aes_cbc_mac.c ../source/sha256.c
 *
 * Expected output (compare to tests/refs.txt):
 *   knuth        169FA000
 *   jenkins_oat  D88778E4
 *   pjw_elf      0FC634A8
 *   sdbm         8FAF7E00
 *   fletcher16   D400
 *   fletcher32   F5F3FF00
 *   fletcher64   9C5C1DDD807F7E81
 *   crc64        29103DD16C9C1449
 *   murmur3_128  85F1E157B5E1DF8181010404FA4A0AA9... no wait, see file
 *   sha3_512     1639C362CD7AE9AE...
 *   pbkdf2       4FACC738DE0C37C2...
 *   aes_cbc_mac  0EF7EA611D82AB32BC1D57FDDD6AB8C7
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "hashes.h"

static uint8_t buf[1024];
static void fill(void) {
    for (int i = 0; i < 1024; i++) buf[i] = (uint8_t)((i * 31 + 7) & 0xFF);
}
static void hex(const char *label, const uint8_t *d, int n) {
    printf("%-14s = ", label);
    for (int i = 0; i < n; i++) printf("%02X", d[i]);
    printf("\n");
}

int main(void) {
    uint8_t o[64];
    fill();

    hash_knuth(buf, 1024, o);          hex("knuth",          o,  4);
    hash_jenkins_oat(buf, 1024, o);    hex("jenkins_oat",    o,  4);
    hash_pjw_elf(buf, 1024, o);        hex("pjw_elf",        o,  4);
    hash_sdbm(buf, 1024, o);           hex("sdbm",           o,  4);
    hash_fletcher16(buf, 1024, o);     hex("fletcher16",     o,  2);
    hash_fletcher32(buf, 1024, o);     hex("fletcher32",     o,  4);
    hash_fletcher64(buf, 1024, o);     hex("fletcher64",     o,  8);
    hash_crc64(buf, 1024, o);          hex("crc64",          o,  8);
    hash_murmur3_128(buf, 1024, o);    hex("murmur3_128",    o, 16);
    hash_sha3_512(buf, 1024, o);       hex("sha3_512",       o, 64);
    hash_pbkdf2_sha256(buf, 1024, o);  hex("pbkdf2",         o, 32);
    hash_aes_cbc_mac(buf, 1024, o);    hex("aes_cbc_mac",    o, 16);
    return 0;
}
