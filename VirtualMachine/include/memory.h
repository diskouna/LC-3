#ifndef __MEMORY__
#define __MEMORY__

#include <stdint.h>
#include <stdbool.h>

#define MEMORY_MAX_SIZE (1 << 16)   // 65536
                                    // LC-3 has 65536 memory 
                                    // locations
uint16_t memory[MEMORY_MAX_SIZE];   // Each location stores
                                    // a 16-bit value.
                                    // It can store up to 
                                    // 128 Kbits of data/instruction

/*mem_write :  write a value to a specific address in the memory */
 void mem_write(uint16_t address, uint16_t value);

/*mem_read : read a value from a specific address in the memory*/
uint16_t mem_read(uint16_t address);
uint16_t swap16(uint16_t n);

/* Memory mapped register */
enum  {
    MR_KBSR = 0xFE00,  // keyboard status register
                       // indicates whether a 
                       // key has been pressed 
    MR_KBDR = 0xFE02   // keyboard data  register
                       // indicates which key was
                       // pressed
};
uint16_t check_key(void);
#endif // __MEMORY__