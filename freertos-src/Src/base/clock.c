#include "stm32g4xx.h"
#include "stm32g4xx_hal_flash.h"
#include "stm32g4xx_hal_rcc.h"

void clock_init(void) {
    RCC_OscInitTypeDef rcc_osc_init = {0};
    RCC_ClkInitTypeDef rcc_clk_init = {0};

    rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    rcc_osc_init.HSIState = RCC_HSI_ON;

    HAL_RCC_OscConfig(&rcc_osc_init);

    rcc_clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK;
    rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
    rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
    rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

    HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_2);
}