#include "rcc.h"

static void RCC_ResetPeripheral(RCC_GPIO_Port periph);
static void RCC_ResetTimer(RCC_TIMER_Timer timer);

static void RCC_EnablePeripheralClock(RCC_GPIO_Port periph);
static void RCC_EnableTimerClock(RCC_TIMER_Timer timer);

static void RCC_ResetPeripheral(RCC_GPIO_Port periph)
{
    RCC_AHB2_RST |= periph;
    RCC_AHB2_RST &= ~periph;
}

static void RCC_ResetTimer(RCC_TIMER_Timer timer)
{
    RCC_APB1_RST |= timer;
    RCC_APB1_RST &= ~timer;
}

static void RCC_EnablePeripheralClock(RCC_GPIO_Port periph)
{
    RCC_AHB2_CLK_EN |= periph;
}

static void RCC_EnableTimerClock(RCC_TIMER_Timer timer)
{
    RCC_APB1_CLK_EN |= timer;
}

void rcc_init(void)
{
   RCC_ResetPeripheral(RCC_GPIO_G);
   RCC_ResetTimer(RCC_TIM_2);

   RCC_EnablePeripheralClock(RCC_GPIO_G); 
   RCC_EnableTimerClock(RCC_TIM_2);
}