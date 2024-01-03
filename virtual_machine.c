#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "stack.h"
#include "helper.h"
#include "constants.h"
#include "decoder.h"

uint16_t reg[8];
uint16_t *prog;
stack s;
uint16_t breakpoint = -1;

uint16_t load(uint16_t n) {
    return prog[n] < REG_START ? prog[n] : reg[prog[n] - REG_START];
}

void move_imm(uint16_t n, uint16_t imm) {
    reg[n % REG_START] = imm;
}

void dump_register(int i) {
    if (i >= 0 && i < 8) {
        printf("$r%d:\t0x%x\t(%d)\n", i, reg[i], reg[i]);
    } else {
        fprintf(stderr, "registers index: [0-7]\n");
    }
}

// return true to continue debugging or false to stop debugging until next point
bool interactive_debugger(uint16_t ip) {
    // welcome to the most scuffed interactive debugger you have ever seen. input validation is minimal and it is super jank. use with caution :)
    // disassemble:     a [instr]
    // break:           b [addr]
    // continue:        c
    // run:             r
    // dump memory:     d [start_addr] [inc]
    // registers:       i [0-7]
    // next instr:      n
    // quit:            q
    // stack:           s
    // dump memory:     x [start_addr] [end_addr]

    char console[1024];
    int ri, ip_iter;
    uint16_t start_addr, end_addr, inc;
    while (true) {
        printf("\n[0x%04x] notgdb> ", ip);
        if (!fgets(console, 1024, stdin)) {
            return false;
        }
        switch (console[0]) {
            case 'a':
                if (sscanf(console, "a %hd", &inc) != 1) {
                    inc = 10;
                }
                ip_iter = ip;
                for (int i = 0; i < inc; i++) {
                    ip_iter += decode_instruction(ip_iter, prog) + 1;
                }
                break;
            case 'b':
                sscanf(console, "b %hx", &breakpoint);
                printf("[INFO] Breakpoint set at 0x%04x\n", breakpoint);
                break;
            case 'c': case 'r':
                return false;
            case 'd':
                if (sscanf(console, "d %hx %hd", &start_addr, &inc) == 2) {
                    for (int i = 0; i < inc; i++) {
                        printf("[0x%04x+0x%02x][0x%04x]\t0x%x\t%u\n", start_addr, i, start_addr + i, prog[start_addr+i], prog[start_addr+i]);
                    }
                }
                break;
            case 'i':
                if (sscanf(console, "i %d", &ri) != 1) {
                    for (int i = 0; i < 8; i++) {
                        dump_register(i);
                    }
                } else {
                    dump_register(ri);
                }
                break;
            case 'n':
                return true;
            case 'q':
                exit(1);
            case 's':
                print_stack(&s);
                break;
            case 'x':
                int argc = sscanf(console, "m %hx %hx", &start_addr, &end_addr);
                if (argc == 1 || argc == 2) {
                    if (argc == 1) {
                        end_addr = start_addr;
                    }
                    for (int i = start_addr; i <= end_addr; i++) {
                        printf("[0x%04x]\t\t0x%x\t%u\n", i, prog[i], prog[i]);
                    }
                }
                break;
            default:
                printf("[ERROR] Unknown command: %s", console);
        }
    }
}

int main() {
    bool debug = true;
    setbuf(stdout, NULL);

    // read binary
    read_binary(&prog);

    // set registers
    memset(reg, 0, sizeof(reg));
    // create stack
    s.size = 0;
    s.head = NULL;

    // fetch–decode–execute cycle
    uint16_t ip = 0;
    bool halt = false;

    uint16_t instr;
    char user_input[0x1000] = "take tablet\nuse tablet\n";
    char *inp_ptr = user_input;
    while (!halt) {
        // debugging
        if (debug || ip == breakpoint) {
            debug = interactive_debugger(ip);
        }
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
                if (*inp_ptr == '\0') {
                    fprintf(stderr, "Tried to read user input but reached EOF.\n");
                    exit(1);
                }
                // end debug
                putchar(*inp_ptr);
                move_imm(prog[ip+1], *inp_ptr);
                inp_ptr++;
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
