#ifndef __CPU__
#define __CPU__

#include <stdint.h>

/* REGISTER */

#define REG_CNT 16      // Number of register
enum reg {              // Type of register
    R0 = 0,
    R1,
    R2,
    R3,
    R4, 
    R5,
    R6,
    R7,     // R0---R7 are general purpose register
    PC,     // Progam counter
    IR,     // Instruction Register
    PSR,    // Progam Status Register 
    USP,    // User Stack Pointer
    SSP     // System Stack Pointer
};

uint16_t regist[REG_CNT];   //Each register is a 16bit value

/* PSR FLAGS */
#define POS (1 << 9)         // Positive 
#define ZRO (1 << 10)        // Zero
#define NEG (1 << 11)        // Negative

#define PRIVI_MOD (1 << 15) // Privileged mode
/* update the PSR flags */
void upd_PSR(enum reg r);

/* Instruction set */

/*    
    Each instruction is a 16 bit value with the left 4 bits 
    storing the opcode.

  inst : _ _ _ _   _ _ _ _  _ _ _ _  _ _ _ _  
          opcode         parameters
*/

#define CHUNK(bits, st, len) ()  // TODO

#define OPC_CNT (1 << 4)    // 16
                            // number of opcodes
enum opcode {
    OPC_BR = 0,       // branch
    OPC_ADD,          // add
    OPC_LD,           // load
    OPC_ST,           // store
    OPC_JSR,          // jump to subroutine
    OPC_AND,          // bitwise AND
    OPC_LDR,          // load register
    OPC_STR,          // store register
    OPC_RTI,          // return from interruption
    OPC_NOT,          // bitwise NOT
    OPC_LDI,          // load indirect
    OPC_STI,          // store indirect
    OPC_JMP,          // jump
    OPC_RES,          // reserved
    OPC_LEA,          // load effective address
    OPC_TRAP          // execute TRAP
};

#define OPCODE(inst)        ((inst) >> 12)
#define DR(inst)  (((inst) >> 9) & 0x0007)
#define SR1(inst) (((inst) >> 6) & 0x0007)
#define SR2(inst)        ((inst) & 0x0007)
#define IMM5FLAG(inst) ((inst) & (1 << 5))
#define IMM5(inst)       ((inst) & 0x001F)
#define OFFSET6(inst)    ((inst) & 0x003F)
#define OFFSET9(inst)    ((inst) & 0x01FF)
#define OFFSET11(inst)   ((inst) & 0x07FF)
#define TRAPVCT8(inst)   ((inst) & 0x00FF)

/* sgn_ext : extend a signed "width" bit number 
             n to a 16bit number
*/
uint16_t sgn_ext(uint16_t n, int width);

/* Arithmetic et logic intructions */

void add(uint16_t inst);
void and(uint16_t inst);
void not(uint16_t inst);

/* Loading instructions */

void ld(uint16_t inst);
void ldi(uint16_t inst);
void ldr(uint16_t inst);
void lea(uint16_t inst);

/* Storing instructions */

void st(uint16_t inst);
void sti(uint16_t inst);
void str(uint16_t inst);

/* Branching instrucion*/

void br(uint16_t inst);
void jmp(uint16_t inst);

void jsr(uint16_t inst);
//void jsrr(uint16_t inst);

void trap(uint16_t inst);

void ret(uint16_t inst);
void rti(uint16_t inst);

/* Trap routines */
#define TRAP_OFFSET (0x0020)
#define TRAP_CNT 6

enum trap {
    TRAP_GETC = 0x20,  // get a character from keyboard, not echoed onto the terminal
    TRAP_OUT,          // output a character
    TRAP_PUTS,         // output a word string
    TRAP_IN,           // get a character from keyboard, echoed onto the terminal
    TRAP_PUTSP,        // output a byte string
    TRAP_HALT          // halt the program
};

void trap_getc(void);
void trap_out(void);
void trap_puts(void);
void trap_in(void);
void trap_putsp(void);
void trap_halt(void);




#endif // __CPU__