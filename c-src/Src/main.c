/**
 ******************************************************************************
 * @file           : main.c
 * @author         : johan[rsa] 
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stddef.h>
#include <stdint.h>
#include "rcc.h"
#include "sys.h"
#include "led.h"
#include "thread.h"
#include "gpio.h"
#include "i2c.h"

/* Tasks */

void t_BlinkLed() {
    led_on(LED3);
}

int main(void)
{
    uint32_t STACK_SIZE = 256;
    (void)STACK_SIZE;

    sys_init();
    rcc_init();
    gpio_init();
    i2c_init();
    led_init();

    // runScheduler();

    t_BlinkLed();
}