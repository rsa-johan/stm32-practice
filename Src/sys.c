#include "sys.h"
#include "rcc.h"
#include <stdint.h>

static inline void check_HSI_ready(void);
static inline void check_HSI_ready() 
{
    while ((RCC_CR & RCC_HSI_ready) == 0U);
}

static void sys_clock_init(void);
static void sys_clock_init()
{
    RCC_CR |= RCC_HSI_on;
    check_HSI_ready();
}

void sys_init()
{
    sys_clock_init();
}
