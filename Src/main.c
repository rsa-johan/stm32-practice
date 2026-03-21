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
#include "thread.h"
#include "rcc.h"
#include "gpio.h"
#include "timer.h"

volatile uint32_t task0Counter = 0;
volatile uint32_t task1Counter = 0;

static void debugLedRun(void *args) {
    (void)args;

    for (;;) {
        task0Counter++;
        for (volatile uint32_t i = 0; i < 1000; ++i) {
            delay(300, DELAY_UNITS_MS);
        }
    }
    yield();
}

int main(void)
{
    sys_clock_init();
    rcc_init();
    gpio_init();

    createTask(debugLedRun, "task0", 512, NULL, 1);

    runScheduler();

    for (;;);
}
