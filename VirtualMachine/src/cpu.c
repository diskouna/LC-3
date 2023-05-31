#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "memory.h"
#include "cpu.h"

 
void upd_PSR(enum reg r)
{
    assert(r <= R7);                // we will only update PSR flag 
                                    // after updaing a general purpose
                                    // register R0---R7
    if (regist[r] == 0)
        regist[PSR] = ZRO;
    else if (regist[r] & (1 << 15)) // the 16th bit is set 
                                    // negative numbers are stored
                                    // using two complement representation
        regist[PSR] = NEG;
    else
        regist[PSR] = POS;
}

uint16_t sgn_ext(uint16_t n, int width)
{
    assert(width <= 16);
    if (width == 16)
        return n;
    uint16_t mask = (1 << (width - 1));
    return (n & mask) ? ((0xFFFF << width) | n) : n;
}

void add(uint16_t inst)
{
    regist[DR(inst)] = regist[SR1(inst)] + (IMM5FLAG(inst) ? 
                                        sgn_ext(IMM5(inst), 5) 
                                        : regist[SR2(inst)]);
    upd_PSR(DR(inst));
}
void and(uint16_t inst)
{
    regist[DR(inst)] = regist[SR1(inst)] & (IMM5FLAG(inst) ? 
                                        sgn_ext(IMM5(inst), 5) 
                                        : regist[SR2(inst)]);
    upd_PSR(DR(inst));
}

void not(uint16_t inst)
{
    regist[DR(inst)] = ~regist[SR1(inst)];
    upd_PSR(DR(inst));
}

void ld(uint16_t inst) 
{
    regist[DR(inst)] = mem_read(regist[PC] + sgn_ext(OFFSET9(inst), 9));
    upd_PSR(DR(inst)); 
}
void ldi(uint16_t inst)
{
    regist[DR(inst)] = mem_read(mem_read(regist[PC] + sgn_ext(OFFSET9(inst), 9)));
    upd_PSR(DR(inst)); 
}
void ldr(uint16_t inst)
{
    regist[DR(inst)] = mem_read(regist[SR1(inst)] + sgn_ext(OFFSET6(inst) ,6));
    upd_PSR(DR(inst)); 
}
void lea(uint16_t inst)
{
    regist[DR(inst)] = regist[PC] + sgn_ext(OFFSET9(inst), 9);
    upd_PSR(DR(inst)); 
}

/* Storing instructions */

void st(uint16_t inst)
{
    mem_write(regist[PC] + sgn_ext(OFFSET9(inst), 9), regist[DR(inst)]);
}
void sti(uint16_t inst)
{
    mem_write(mem_read(regist[PC] + sgn_ext(OFFSET9(inst), 9)), regist[DR(inst)]);
}   
void str(uint16_t inst)
{
    mem_write(regist[SR1(inst)] + sgn_ext(OFFSET6(inst), 6), regist[DR(inst)]);
    // regist[SR1] !!! SR1
}

/* Branching instrucion*/

void br(uint16_t inst)
{
    if ((inst & 0x0E00) & regist[PSR]) {            // nzp & NZP
        regist[PC] += sgn_ext(OFFSET9(inst), 9);    // nzp : inst[11..9]
                                                    // NZP : reg[PSR][11..9]
    }
}
void jmp(uint16_t inst)
{
    regist[PC] = regist[SR1(inst)];
}
void jsr(uint16_t inst)
{
    regist[R7] = regist[PC];
    if (inst & (1 << 11)) {
        regist[PC] += sgn_ext(OFFSET11(inst), 11);
    } else {
        regist[PC] = regist[SR1(inst)];
    }   
}
//void jsrr(uint16_t inst);

void ret(uint16_t inst)
{
    regist[PC] = regist[R7];

}
void rti(uint16_t inst)
{
    if (inst & PRIVI_MOD) {
        ;
    } else {
        regist[PC] = mem_read(regist[6]);
        regist[6]++;
        uint16_t tmp = mem_read(regist[6]);
        regist[6]++;
        regist[PSR] = tmp;
    }
}


void trap_getc(void)
{
    regist[R0] = (uint16_t)getchar();
    upd_PSR(R0);
}
void trap_out(void)
{
    fprintf(stdout, "%c", (char)regist[R0]);
    fflush(stdout);
}
void trap_puts(void)
{
    uint16_t *p = memory + regist[R0];
    while (*p) {
        fprintf(stdout, "%c", (char)*p);
        p++;
    }
    fflush(stdout);
}
void trap_in(void)
{
    fprintf(stdout, "enter a character: ");
    char c = getchar();
    putc(c, stdout);
    fflush(stdout);
    regist[R0]= (uint16_t)c;
    upd_PSR(R0);
}
void trap_putsp(void)
{
    uint16_t *p = memory + regist[R0];
    while (*p) {
        char c1 = (*p) & 0x00FF;
        putc(c1, stdout);
        char c2 = (*p) >> 8;
        if (c2) putc(c2, stdout);
        p++;
    }
    fflush(stdout);
}
void trap_halt(void)
{
    extern bool running;
    puts("HALT");
    fflush(stdout);
    running = false;
}

void (*trap_exec[TRAP_CNT])(void) = {
    trap_getc,
    trap_out,
    trap_puts,
    trap_in,
    trap_putsp,
    trap_halt,
};

void trap(uint16_t inst)
{
    regist[R7] = regist[PC];
    trap_exec[TRAPVCT8(inst) - TRAP_OFFSET]();
}

/* Array of the opcode execution function */
void (*opcode_exec [OPC_CNT])(uint16_t inst) = {
    br,
    add,
    ld,
    st,
    jsr,
    and,
    ldr,
    str,
    rti,
    not,
    ldi,
    sti,
    jmp,
    NULL,
    lea,
    trap
};


// array of operation ??