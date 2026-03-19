#include "timer.h"
#include <cstdint>

void delay(uint32_t units, delay_units_t unit)
{
    timer_t timer = TIM_2;
    uint32_t time_units = units * (uint32_t)unit;
    TIM_DIER(timer) = TIM_DIER_UIE;
}

void timer_init(void)
{
    timer_t timer = TIM_2;
    uint32_t prescaler = 16 - 1;
    TIM_PSC(timer) = prescaler;

    uint32_t auto_reload_value = 1000 - 1;
    TIM_ARR(timer) = auto_reload_value;
}