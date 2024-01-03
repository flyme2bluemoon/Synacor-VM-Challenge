#include <stdint.h>
#include <ctype.h>
#include <stdio.h>

#include "constants.h"

void print_value(int n, int base) {
    if (n < REG_START) {
        if (base == 0x10) printf("0x%X", n);
        else if (base == 0) {
            if (isprint(n)) printf("'%c'", n);
            else printf("'\\x%02X'", n);
        }
        else printf("%d", n);
    } else {
        printf("r%d", n % REG_START);
    }
    putchar(' ');
}

int decode_instruction(uint16_t ip, uint16_t *prog) {
    uint16_t instr = prog[ip];
    char *op;
    int argc;
    int base = 0x10;

    switch (instr) {
        case 0: // halt
            op = "halt";
            argc = 0;
            break;
        case 1: // set
            op = "set";
            argc = 2;
            break;
        case 2: // push
            op = "push";
            argc = 1;
            base = 10;
            break;
        case 3: // pop
            op = "pop";
            argc = 1;
            break;
        case 4: // eq
            op = "eq";
            argc = 3;
            base = 10;
            break;
        case 5: // gt
            op = "gt";
            argc = 3;
            base = 10;
            break;
        case 6: // jmp
            op = "jmp";
            argc = 1;
            break;
        case 7: // jt
            op = "jt";
            argc = 2;
            break;
        case 8: //jf
            op = "jf";
            argc = 2;
            break;
        case 9: // add
            op = "add";
            argc = 3;
            base = 10;
            break;
        case 10: // mult
            op = "mult";
            argc = 3;
            base = 10;
            break;
        case 11: // mod
            op = "mod";
            argc = 3;
            base = 10;
            break;
        case 12: // and
            op = "and";
            argc = 3;
            break;
        case 13: // or
            op = "or";
            argc = 3;
            break;
        case 14: // not
            op = "not";
            argc = 2;
            break;
        case 15: // rmem
            op = "rmem";
            argc = 2;
            break;
        case 16: // wmem
            op = "wmem";
            argc = 2;
            break;
        case 17: // call
            op = "call";
            argc = 1;
            break;
        case 18: // ret
            op = "ret";
            argc = 0;
            break;
        case 19: // out
            op = "out";
            argc = 1;
            base = 0;
            break;
        case 20: // in
            op = "in";
            argc = 1;
            break;
        case 21: // noop
            op = "noop";
            argc = 0;
            break;
        default:
            return 0;
    }
    printf("0x%04X:\t\t%s ", ip, op);
    if (argc >= 1) {
        print_value(prog[ip+1], base);
        if (argc >= 2) {
            print_value(prog[ip+2], base);
            if (argc >= 3) {
                print_value(prog[ip+3], base);
            }
        }
    }
    putchar('\n');
    return argc;
}
