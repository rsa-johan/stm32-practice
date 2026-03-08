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

volatile uint32_t task0Counter = 0;
volatile uint32_t task1Counter = 0;

static void task0(void *arg)
{
    (void)arg;

    for (;;) {
        task0Counter++;
        for (volatile uint32_t i = 0; i < 1000; ++i) {
        }
        yield();
    }
}

static void task1(void *arg)
{
    (void)arg;

    for (;;) {
        task1Counter++;
        for (volatile uint32_t i = 0; i < 1000; ++i) {
        }
        yield();
    }
}

int main(void)
{
    createTask(task0, "task0", 512, NULL, 1);
    createTask(task1, "task1", 512, NULL, 1);

    runScheduler();

    for (;;);
}
