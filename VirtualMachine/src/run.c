#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>
#include "loader.h"
#include "cpu.h"
#include "memory.h"

#define PC_START (0x3000)
struct termios original_tio;

void disable_input_buffering()
{
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void restore_input_buffering()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}



void handle_interrupt(int signal)
{
    restore_input_buffering();
    printf("\n");
    exit(-2);
}
bool running = true;

extern void (*opcode_exec [OPC_CNT])(uint16_t inst);
extern void (*trap_exec[TRAP_CNT])(void);

enum
{
    OP_BR = 0, /* branch */
    OP_ADD,    /* add  */
    OP_LD,     /* load */
    OP_ST,     /* store */
    OP_JSR,    /* jump registister */
    OP_AND,    /* bitwise and */
    OP_LDR,    /* load registister */
    OP_STR,    /* store registister */
    OP_RTI,    /* unused */
    OP_NOT,    /* bitwise not */
    OP_LDI,    /* load indirect */
    OP_STI,    /* store indirect */
    OP_JMP,    /* jump */
    OP_RES,    /* reserved (unused) */
    OP_LEA,    /* load effective address */
    OP_TRAP    /* execute trap */
};

int main(int argc, char * argv[])
{

    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    if (argc != 2) {
        fprintf(stderr, "invalid syntax! \nsyntax: %s [bin_file] ...\n", argv[0]);
        exit(1); 
    }

    
    
    regist[PC] = read_bin(argv[1]); 
    //printf("\n %08X \n", registist[PC]);
    regist[PSR] = 0;
    
    running = true;

    while (running) {
        uint16_t instr = mem_read(regist[PC]++);   // fetch + incremente
                                                  // the PC
        
        //printf("\n %x \n", OPCODE(instr));

        opcode_exec[OPCODE(instr)](instr);          // decode + execute        

        
        
    }
    restore_input_buffering();

    return 0;
}