#include "sys.h"
#include "rcc.h"

static void sys_clock_init(void);

static void sys_clock_init()
{
    RCC_ClockSource clockSource = RCC_HSI;
    RCC_CR |= RCC_CR_MASK(clockSource);
}

void sys_init()
{
    sys_clock_init();
}
