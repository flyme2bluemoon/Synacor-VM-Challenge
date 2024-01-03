#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "helper.h"

#define REG_START 32768

uint16_t *prog;

void print_value(int n) {
    if (n < REG_START) {
        printf("%d\t", n);
    } else {
        printf("r%d\t", n % REG_START);
    }
}

void decode_instruction(char *op, int argc, uint16_t ip) {
    printf("0x%04x:\t\t%s\t", ip, op);
    if (argc >= 1) {
        print_value(prog[ip+1]);
        if (argc >= 2) {
            print_value(prog[ip+2]);
            if (argc >= 3) {
                print_value(prog[ip+3]);
            }
        }
    }
    putchar('\n');
}

int main() {
    // read binary
    long size = read_binary(&prog);

    uint16_t instr;
    for (long ip = 0; ip < size; ip++) {
        instr = prog[ip];
        // execute
        switch (instr) {
            case 0: // halt
                decode_instruction("halt", 0, ip);
                break;
            case 1: // set
                decode_instruction("set", 2, ip);
                ip += 2;
                break;
            case 2: // push
                decode_instruction("push", 1, ip);
                ip += 1;
                break;
            case 3: // pop
                decode_instruction("pop", 1, ip);
                ip += 1;
                break;
            case 4: // eq
                decode_instruction("eq", 3, ip);
                ip += 3;
                break;
            case 5: // gt
                decode_instruction("gt", 3, ip);
                ip += 3;
                break;
            case 6: // jmp
                decode_instruction("jmp", 1, ip);
                ip += 1;
                break;
            case 7: // jt
                decode_instruction("jt", 2, ip);
                ip += 2;
                break;
            case 8: //jf
                decode_instruction("jf", 2, ip);
                ip += 2;
                break;
            case 9: // add
                decode_instruction("add", 3, ip);
                ip += 3;
                break;
            case 10: // mult
                decode_instruction("mult", 3, ip);
                ip += 3;
                break;
            case 11: // mod
                decode_instruction("mod", 3, ip);
                ip += 3;
                break;
            case 12: // and
                decode_instruction("and", 3, ip);
                ip += 3;
                break;
            case 13: // or
                decode_instruction("or", 3, ip);
                ip += 3;
                break;
            case 14: // not
                decode_instruction("not", 2, ip);
                ip += 2;
                break;
            case 15: // rmem
                decode_instruction("rmem", 2, ip);
                ip += 2;
                break;
            case 16: // wmem
                decode_instruction("wmem", 2, ip);
                ip += 2;
                break;
            case 17: // call
                decode_instruction("call", 1, ip);
                ip += 1;
                break;
            case 18: // ret
                decode_instruction("ret", 0, ip);
                break;
            case 19: // out
                decode_instruction("out", 1, ip);
                ip += 1;
                break;
            case 20: // in
                decode_instruction("in", 1, ip);
                ip += 1;
                break;
            case 21: // noop
                decode_instruction("noop", 0, ip);
                break;
            // default:
            //     fprintf(stderr, "invalid instr: %d\n", instr);
            //     exit(1);
        }
    }

    // free memory
    free(prog);

    return 0;
}
