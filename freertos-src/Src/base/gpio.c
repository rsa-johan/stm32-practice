#include "gpio.h"
#include "stm32g4xx_hal_gpio.h"

void gpio_init() {

    /* RCC init */
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

}