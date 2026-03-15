/* rcc.c - RCC (Reset and Clock Control) implementation for STM32 microcontroller. */

static void RCC_ResetPeripheral(enum RCC_Peripheral periph)
{
    RCC_AHB2_RST |= RCC_PERIPH_MASK(periph);
    RCC_AHB2_RST &= ~RCC_PERIPH_MASK(periph);
}

static void RCC_ResetTimer(enum RCC_APB1_Bit timer)
{
    RCC_APB1_RST |= (1U << timer);
    RCC_APB1_RST &= ~(1U << timer);
}

static void RCC_EnablePeripheralClock(enum RCC_Peripheral periph)
{
    RCC_AHB2_CLK_EN |= RCC_PERIPH_MASK(periph);
}

static void RCC_EnableTimerClock(enum RCC_APB1_Bit timer)
{
    RCC_APB1_CLK_EN |= (1U << timer);
}

void rcc_init(void)
{
   RCC_ResetPeripheral(RCC_PERIPH_GPIOG);
   RCC_EnablePeripheralClock(RCC_PERIPH_GPIOG); 
}