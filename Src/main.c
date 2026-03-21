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
#include "led.h"
#include "thread.h"
#include "rcc.h"
#include "gpio.h"
#include "timer.h"

static void debugLedRun(void *args);
static void debugLedRun(void *args) {
    (void)args;

    for (;;) {
        for (volatile uint32_t i = 0; i < 1000; ++i) {
            led_on(LedPort1, LED1);
            delay(2000, DELAY_UNITS_MS);
            led_off(LedPort1, LED1);
        }
    }
    yield();
}

int main(void)
{
    uint32_t STACK_SIZE = 256;
    /*
    sys_clock_init();
    */
    rcc_init();
    gpio_init();

    createTask(debugLedRun, "task0", STACK_SIZE, NULL, 1);

    runScheduler();

    for (;;);
}
