#include <stdio.h>
#include "FreeRTOS.h"
#include "signals.h"
#include "base/clock.h"
#include "system_stm32g4xx.h"

int main(void)
{
    SystemInit();
    clock_init();
    return 0;
}