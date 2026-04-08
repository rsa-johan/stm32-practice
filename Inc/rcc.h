#ifndef RCC_H
#define RCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define RCC_BASE 0X40021000U

#define RCC_CR_OFFSET 0x00U
#define RCC_CFGR_OFFSET 0x08U

#define RCC_AHB2_RST_OFFSET 0x2CU
#define RCC_AHB2_CLK_EN_OFFSET 0x4CU

#define RCC_APB1_RST_OFFSET 0x38U
#define RCC_APB1_CLK_EN_OFFSET 0x58U

#define RCC_CR (*(volatile uint32_t *)(RCC_BASE + RCC_CR_OFFSET))
#define RCC_CFGR (*(volatile uint32_t *)(RCC_BASE + RCC_CFGR_OFFSET))

#define RCC_AHB2_RST (*(volatile uint32_t *)(RCC_BASE + RCC_AHB2_RST_OFFSET))
#define RCC_AHB2_CLK_EN (*(volatile uint32_t *)(RCC_BASE + RCC_AHB2_CLK_EN_OFFSET))

#define RCC_APB1_RST (*(volatile uint32_t *)(RCC_BASE + RCC_APB1_RST_OFFSET))
#define RCC_APB1_CLK_EN (*(volatile uint32_t *)(RCC_BASE + RCC_APB1_CLK_EN_OFFSET))

typedef enum {
    RCC_GPIO_A = 1U << 0,
    RCC_GPIO_B = 1U << 1,
    RCC_GPIO_C = 1U << 2,
    RCC_GPIO_D = 1U << 3,
    RCC_GPIO_E = 1U << 4,
    RCC_GPIO_F = 1U << 5,
    RCC_GPIO_G = 1U << 6,
} RCC_GPIO_Port;

typedef enum {
    RCC_TIM_2 = 1U << 0,
    RCC_TIM_3 = 1U << 1,
    RCC_TIM_4 = 1U << 2,
    RCC_TIM_5 = 1U << 3,
    RCC_TIM_6 = 1U << 4,
    RCC_TIM_7 = 1U << 5,
} RCC_TIMER_Timer;

typedef enum {
    RCC_I2C_1 = 1U << 21,
    RCC_I2C_2 = 1U << 22,
    RCC_I2C_3 = 1U << 23,
} RCC_I2C_Peripheral;

typedef enum {
    RCC_HSI_on = 1U << 8,
    RCC_HSI_ready = 1U << 10,
    RCC_HSE_on = 1U << 16,
    RCC_HSE_ready = 1U << 17,
    RCC_PLL_on = 1U << 24,
    RCC_PLL_ready = 1U << 25,
} RCC_ClockState;

typedef enum {
    RCC_HSI_PRESCALER_DIV_1 = 0U,
    RCC_HSI_PRESCALER_DIV_2 = 1U,
    RCC_HSI_PRESCALER_DIV_4 = 2U,
    RCC_HSI_PRESCALER_DIV_8 = 3U,
} RCC_HSI_Prescaler;

typedef enum {
    RCC_SYSCLK_HSI_on = 1U << 0,
    RCC_SYSCLK_HSE_on = 2U << 0,
    RCC_SYSCLK_PLL_on = 3U << 0,
    RCC_SYSCLK_RST_on = ~(3U << 0),
} RCC_SYSCLK_Source;

typedef enum {
    RCC_SYSCLK_RST_ready = 0U << 2,
    RCC_SYSCLK_HSI_ready = 1U << 2,
    RCC_SYSCLK_HSE_ready = 2U << 2,
    RCC_SYSCLK_PLL_ready = 3U << 2,
} RCC_SYSCLK_Source_Status;


void rcc_init(void);

#ifdef __cplusplus
}
#endif

#endif /* RCC_H */
