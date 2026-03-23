#include "sys.h"
#include "rcc.h"

static inline void check_HSI_ready(void);
static inline void check_HSI_ready() 
{
    while ((RCC_CR & RCC_HSI_ready) == 0U);
}

static inline void check_sysclk_ready() 
{
    while ((RCC_CR & RCC_SYSCLK_HSI_ready) == 0U);
}

static void sys_clock_init(void);
static void sys_clock_init()
{
    RCC_ClockState hsiOn = RCC_HSI_on;
    RCC_CR |= hsiOn;
    check_HSI_ready();
    RCC_CFGR |= RCC_SYSCLK_HSI_on;
    check_sysclk_ready();
}

void sys_init()
{
    sys_clock_init();
}
