#ifndef RCC_H
#define RCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define RCC_BASE (*(volatile uint32_t *)0X40021000U)

#define RCC_CR_OFFSET 0x00U

#define RCC_AHB2_RST_OFFSET 0x2CU
#define RCC_AHB2_CLK_EN_OFFSET 0x4CU

#define RCC_APB1_RST_OFFSET 0x38U
#define RCC_APB1_CLK_EN_OFFSET 0x58U

#define RCC_CR (*(volatile uint32_t *)(RCC_BASE + RCC_CR_OFFSET))

#define RCC_AHB2_RST (*(volatile uint32_t *)(RCC_BASE + RCC_AHB2_RST_OFFSET))
#define RCC_AHB2_CLK_EN (*(volatile uint32_t *)(RCC_BASE + RCC_AHB2_CLK_EN_OFFSET))

#define RCC_APB1_RST (*(volatile uint32_t *)(RCC_BASE + RCC_APB1_RST_OFFSET))
#define RCC_APB1_CLK_EN (*(volatile uint32_t *)(RCC_BASE + RCC_APB1_CLK_EN_OFFSET))


#define RCC_GPIO_MASK(port) ((uint32_t)(1U << (port)))
#define RCC_TIMER_MASK(timer) ((uint32_t)(1U << (timer)))
#define RCC_CR_MASK(bit) ((uint32_t)(1U << (bit)))

typedef enum {
    RCC_NONE = 0,
    RCC_GPIO_A = 1,
    RCC_GPIO_B = 2,
    RCC_GPIO_C = 3,
    RCC_GPIO_D = 4,
    RCC_GPIO_E = 5,
    RCC_GPIO_F = 6,
    RCC_GPIO_G = 7,
} RCC_GPIO_Bit;

typedef enum {
    RCC_TIM_2 = 0,
    RCC_TIM_3 = 1,
    RCC_TIM_4 = 2,
    RCC_TIM_5 = 3,
    RCC_TIM_6 = 4,
    RCC_TIM_7 = 5,
} RCC_TIMER_Bit;

typedef enum {
    RCC_HSI = 8,
    RCC_HSE = 16,
    RCC_PLL = 24,
} RCC_ClockSource;

void rcc_init(void);

#ifdef __cplusplus
}
#endif

#endif /* RCC_H */