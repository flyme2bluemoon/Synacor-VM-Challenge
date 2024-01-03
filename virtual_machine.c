#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "stack.h"
#include "helper.h"
#include "constants.h"

uint16_t reg[8];
uint16_t *prog;

uint16_t load(uint16_t n) {
    return prog[n] < REG_START ? prog[n] : reg[prog[n] - REG_START];
}

void move_imm(uint16_t n, uint16_t imm) {
    reg[n % REG_START] = imm;
}

int main() {
    // read binary
    read_binary(&prog);

    // set registers
    memset(reg, 0, sizeof(reg));
    // create stack
    stack s;
    s.size = 0;
    s.head = NULL;

    // fetch–decode–execute cycle
    uint16_t ip = 0;
    bool halt = false;

    uint16_t instr;
    char buf[1024];
    while (!halt) {
        // fetch
        instr = load(ip);
        // execute
        switch (instr) {
            case 0: // halt
                halt = true;
                break;
            case 1: // set
                move_imm(prog[ip+1], load(ip+2));
                ip += 2;
                break;
            case 2: // push
                push(&s, load(ip+1));
                ip += 1;
                break;
            case 3: // pop
                move_imm(prog[ip+1], pop(&s));
                ip += 1;
                break;
            case 4: // eq
                move_imm(prog[ip+1], load(ip+2) == load(ip+3) ? 1 : 0);
                ip += 3;
                break;
            case 5: // gt
                move_imm(prog[ip+1], load(ip+2) > load(ip+3) ? 1 : 0);
                ip += 3;
                break;
            case 6: // jmp
                ip = load(ip+1) - 1;
                break;
            case 7: // jt
                if (load(ip+1) != 0) ip = load(ip+2) - 1;
                else ip += 2;
                break;
            case 8: //jf
                if (load(ip+1) == 0) ip = load(ip+2) - 1;
                else ip += 2;
                break;
            case 9: // add
                move_imm(prog[ip+1], (load(ip+2) + load(ip+3)) % REG_START);
                ip += 3;
                break;
            case 10: // mult
                move_imm(prog[ip+1], (load(ip+2) * load(ip+3)) % REG_START);
                ip += 3;
                break;
            case 11: // mod
                move_imm(prog[ip+1], (load(ip+2) % load(ip+3)));
                ip += 3;
                break;
            case 12: // and
                move_imm(prog[ip+1], load(ip+2) & load(ip+3));
                ip += 3;
                break;
            case 13: // or
                move_imm(prog[ip+1], load(ip+2) | load(ip+3));
                ip += 3;
                break;
            case 14: // not
                move_imm(prog[ip+1], (~load(ip+2) & 0x7fff));
                ip += 2;
                break;
            case 15: // rmem
                move_imm(prog[ip+1], prog[load(ip+2)]);
                ip += 2;
                break;
            case 16: // wmem
                prog[load(ip+1)] = load(ip+2);
                ip += 2;
                break;
            case 17: // call
                push(&s, ip+2);
                ip = load(ip+1)-1;
                break;
            case 18: // ret
                ip = pop(&s) - 1;
                break;
            case 19: // out
                putchar(load(ip+1));
                ip += 1;
                break;
            case 20: // in
                fgets(buf, 1024, stdin);
                int i;
                for (i = 0; buf[i] != '\n'; i++) {
                    prog[load(ip+1)+i] = buf[i];
                }
                prog[load(ip+1)+i] = '\0';
                ip += 1;
                break;
            case 21: // noop
                break;
            default:
                fprintf(stderr, "invalid instr: %d\n", instr);
                exit(1);
        }
        ip++;
    }

    // free memory
    free(prog);
    freeStack(&s);

    return 0;
}
