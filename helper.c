#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "helper.h"
#include "constants.h"

// reads binary and puts the address into *mem
// returns size of binary in words (16 bits/2 bytes)
long read_binary(uint16_t **mem) {
    uint16_t *prog;

    // open binary
    FILE *binary = fopen(BINARY_FILE, "rb");
    if (binary == NULL) {
        fprintf(stderr, "Unable to load %s\n", BINARY_FILE);
        exit(1);
    }


    // determine binary size
    fseek(binary, 0, SEEK_END);
    long bin_size = ftell(binary);
    rewind(binary);

    prog = malloc(bin_size > 0x8000 ? bin_size : 0x8000);
    fread(prog, sizeof(uint16_t), bin_size, binary);

    // close binary
    fclose(binary);

    *mem = prog;

    return bin_size / 2;
}
