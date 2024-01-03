#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "helper.h"
#include "decoder.h"

#define REG_START 32768

uint16_t *prog;

int main() {
    // read binary
    long size = read_binary(&prog);

    for (long ip = 0; ip < size; ip++) {
        ip += decode_instruction(ip, prog);
    }

    // free memory
    free(prog);

    return 0;
}
