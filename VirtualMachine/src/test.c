#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint16_t program[] = {
    0xF026,
    0x1220,
    0xF026,
    0x1240,
    0x1060,
    0xF027,
    0xF025
};

int main(void)
{
    char *filename = "bin.obj";
    FILE *out = fopen(filename, "wb");
    if (out == NULL) {
        fprintf(stderr, "cannot create the program \n");
        exit(EXIT_FAILURE);
    }

    size_t writ = fwrite(program, sizeof(uint16_t), sizeof(program)/sizeof(program[0]), out);
    printf("Written size_t = %lu to file %s\n", writ, filename);
    
    fclose(out);

    return 0;
}