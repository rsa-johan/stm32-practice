#include <stdint.h>

/* Hard Fault Handler - prints fault information for debugging */
void HardFault_Handler(void) {
    __asm volatile (
        "MOVS   R0, #4                  \n"
        "MOV    R1, LR                  \n"
        "TST    R0, R1                  \n"
        "BEQ    _MSP                    \n"
        "MRS    R0, PSP                 \n"
        "B      _HALT                   \n"
        "_MSP:                          \n"
        "MRS    R0, MSP                 \n"
        "_HALT:                         \n"
        "BKPT   #0                      \n"
        "B      _HALT                   \n"
    );
}

/* Usage Fault Handler */
void UsageFault_Handler(void) {
    __asm volatile (
        "BKPT   #0                      \n"
        "B      UsageFault_Handler      \n"
    );
}

/* Bus Fault Handler */
void BusFault_Handler(void) {
    __asm volatile (
        "BKPT   #0                      \n"
        "B      BusFault_Handler        \n"
    );
}
