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
#include "timer.h"

void LedRun0(void *args);
void LedRun1(void *args);

void LedRun0(void *args) 
{
    (void)args;

    for(uint32_t i = 0; i < 1000; i++) {
        led_toggle(LED1);
        delay(1000, DELAY_UNITS_MS);
    }
    yield();
}

void LedRun1(void *args)
{
    (void)args;
    for(uint32_t i = 0; i < 1000; i++) {
        led_toggle(LED2);
        delay(1000, DELAY_UNITS_MS);
    }
    yield();
}

void temp_callback(void);
void temp_callback()
{
    led_off(LED1);
    led_on(LED2);
}

int main(void)
{
    uint32_t STACK_SIZE = 256;
    (void)STACK_SIZE;

    sys_init();
    rcc_init();
    gpio_init();
    led_init();
    timer_init();

    createTask(LedRun0, "task0", STACK_SIZE, NULL, 1);
    createTask(LedRun1, "task1", STACK_SIZE, NULL, 2);

    runScheduler();
}
