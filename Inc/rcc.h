#ifndef RCC_H
#define RCC_H

#ifdef __cplusplus
extern "C" {
#endif

#define RCC_BASE (*(volatile uint32_t *)0X40021000U)

#define RCC_AHB2_RST_OFFSET 0x2CU
#define RCC_AHB2_CLK_EN_OFFSET 0x4CU

#define RCC_APB1_RST_OFFSET 0x38U
#define RCC_APB1_CLK_EN_OFFSET 0x54U

#define RCC_AHB2_RST (*(volatile uint32_t *)(RCC_BASE + RCC_AHB2_RST_OFFSET))
#define RCC_AHB2_CLK_EN (*(volatile uint32_t *)(RCC_BASE + RCC_AHB2_CLK_EN_OFFSET))

#define RCC_PERIPH_MASK(port) ((uint32_t)(1U << (port)))

enum RCC_Peripheral {
    RCC_PERIPH_NONE = 0,
    RCC_PERIPH_GPIOA = 1,
    RCC_PERIPH_GPIOB = 2,
    RCC_PERIPH_GPIOC = 3,
    RCC_PERIPH_GPIOD = 4,
    RCC_PERIPH_GPIOE = 5,
    RCC_PERIPH_GPIOF = 6,
    RCC_PERIPH_GPIOG = 7,
};

enum RCC_APB1_Bit {
    RCC_APB1_TIM2 = 0,
    RCC_APB1_TIM3 = 1,
    RCC_APB1_TIM4 = 2,
    RCC_APB1_TIM5 = 3,
    RCC_APB1_TIM6 = 4,
    RCC_APB1_TIM7 = 5,
};

static void RCC_ResetPeripheral(enum RCC_Peripheral periph);
static void RCC_ResetTimer(enum RCC_APB1_Bit timer);

static void RCC_EnablePeripheralClock(enum RCC_Peripheral periph);
static void RCC_EnableTimerClock(enum RCC_APB1_Bit timer);

void rcc_init(void);

#ifdef __cplusplus
}
#endif

#endif /* RCC_H */