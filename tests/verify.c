/*
 * verify.c — host-side correctness check for the 6 NDS-only algorithms.
 * Compiles each .c with the host gcc and exercises it on the same
 * (i*31+7) & 0xFF buffer used by the on-device benchmark, then prints
 * a digest. Compare against the reference values in tests/refs.txt.
 *
 * Build & run:
 *   gcc -O2 -I../include -o verify verify.c \
 *       ../source/sha512.c ../source/sha3.c ../source/ripemd160.c \
 *       ../source/hmac_sha256.c ../source/siphash.c ../source/xxhash64.c \
 *       ../source/sha256.c
 *   ./verify
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
    printf("%-12s = ", label);
    for (int i = 0; i < n; i++) printf("%02X", d[i]);
    printf("\n");
}

int main(void) {
    uint8_t d64[64];
    fill();

    hash_sha512(buf, 1024, d64);            hex("SHA-512",      d64, 64);
    hash_sha3_256(buf, 1024, d64);          hex("SHA-3-256",    d64, 32);
    hash_ripemd160(buf, 1024, d64);         hex("RIPEMD-160",   d64, 20);
    hash_hmac_sha256(buf, 1024, d64);       hex("HMAC-SHA256",  d64, 32);
    hash_siphash24(buf, 1024, d64);         hex("SipHash-2-4",  d64,  8);
    hash_xxh64(buf, 1024, d64);             hex("xxHash64",     d64,  8);

    return 0;
}
