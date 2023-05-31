#include <stdio.h>
#include <stdlib.h>
#include "loader.h"
#include "memory.h"

extern uint16_t memory[MEMORY_MAX_SIZE];


uint16_t read_bin(char * filename)
{
    FILE *in = fopen(filename, "rb");
    if (in == NULL){
        fprintf(stderr, "Failed to open  file %s\n", filename);
        exit(1);
    }
    uint16_t pc_start;
    fread(&pc_start, sizeof(uint16_t), 1, in);
    pc_start = swap16(pc_start); // little endian to big endian
   
    uint16_t max_read = MEMORY_MAX_SIZE - pc_start;
    uint16_t* p = memory + pc_start;
    size_t size = fread(p, sizeof(uint16_t), max_read, in);
    
    while (size--) {
        *p = swap16(*p);
        p++;
    }

    fclose(in);
    return pc_start;
}